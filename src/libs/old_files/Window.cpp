#include "Window.hpp"

#include <curses.h>
#include <panel.h>

#include <cstdbool>

#define GET_PANEL_USER_DATA(obj)  (PanelUserData *) panel_userptr(obj)

/* using namespace graphics; */

typedef struct user_data_list_ {
    int counter;
    PanelUserData * begin;
    PanelUserData * end;
} UserDataList;

static UserDataList panelList;

/* unsigned Window::id = 0; */

Window::Window(void) {
    getmaxyx(stdscr, this->height, this->width);
    this->parent = newwin(this->height, this->width, 0, 0);

    this->type = BORDERED_WIDGET;

    this->panel = new_panel(this->parent);
    PanelUserData * data = new PanelUserData;
    data->hidden = true;
    data->focused = false;

    if (!panelList.begin) {
        panelList.begin = data;
        panelList.end = data;
    } else {
        panelList.end->next = data;
        panelList.end = data;
    }

    data->next = NULL;
    data->id = panelList.counter++;

    set_panel_userptr (this->panel, data);

    box(this->parent, 0, 0);

    update_panels();

    hide_panel(this->panel);

}

Window::Window(size_t height, size_t width, unsigned x, unsigned y) {
    /* getmaxyx(stdscr, this->height, this->width); */
    this->parent = newwin(height, width, y, x);

    this->type = BORDERED_WIDGET;

    box(this->parent, 0, 0);

    this->panel = new_panel(this->parent);
    PanelUserData * data = new PanelUserData;
    data->hidden = true;
    data->focused = false;

    if (!panelList.begin) {
        panelList.begin = data;
        panelList.end = data;
    } else {
        panelList.end->next = data;
        panelList.end = data;
    }

    data->next = NULL;
    panelList.counter++;

    set_panel_userptr (this->panel, data);

    hide_panel (this->panel);
    update_panels();

}

Window::Window(size_t height, size_t width, unsigned x, unsigned y, string title) {
    /* getmaxyx(stdscr, this->height, this->width); */
    this->parent = newwin(height, width, y, x);
    box(this->parent, 0, 0);

    this->type = DECORATED_WINDOW;
    this->decoration_padding = 3;

    this->addTitleBar(this->parent, title, 2);

    this->panel = new_panel(this->parent);
    PanelUserData * data = new PanelUserData;
    data->hidden = true;
    data->focused = false;

    if (!panelList.begin) {
        panelList.begin = data;
        panelList.end = data;
    } else {
        panelList.end->next = data;
        panelList.end = data;
    }

    data->next = NULL;
    panelList.counter++;

    set_panel_userptr (this->panel, data);

    hide_panel (this->panel);
    update_panels();

}

Window::~Window() {
    PanelUserData * prev;
    PanelUserData * temp = (PanelUserData *) panel_userptr(this->panel);
    for (PanelUserData * iter = panelList.begin; iter; iter = iter->next) {
        if (iter->id == temp->id) {
            prev->next = iter->next;
            temp = iter;
            break;
        }
        prev = iter;
    }

    free(temp);
    delwin(getParent());

}

WINDOW *
Window::getParent() {
    return this->parent;
}

PANEL *
Window::getPanel() {
    return this->panel;
}

bool
Window::refresh() {
    if (wrefresh(this->parent) != OK) {
        return false;
    }

    WindowChild * widget;
    list<WindowChild *>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); iter++) {
        widget = (* iter);
        touchwin (widget->child);
    }

    hide_panel(this->getPanel());
    update_panels();
    doupdate();
    if (this->hidden()) {
#ifdef DEBUG_
        printw("Hidden\n");
#endif
        return true;
    }

    show_panel(getPanel());
    update_panels();
    doupdate();

    /* wrefresh(getParent()); */


    return true;
}

void
Window::refresh(unsigned id) {
    WindowChild * widget;
    list<WindowChild *>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); iter++) {
        widget = (* iter);
        if (widget->id == id) {
            touchwin (widget->child);
            wrefresh (widget->child);
            break;
        }
    }

}

bool
Window::display() {
    bool ans;
    if (this->hidden()) {
        ans = show_panel(getPanel());
    }

    this->focus();

    this->refresh();

    update_panels();
    doupdate();

    return ans;
}

bool
Window::hide() {

    if (this->hidden())
        return true;

    PanelUserData * data = GET_PANEL_USER_DATA(getPanel());

    data->hidden = true;
    data->focused = false;

    if (hide_panel(getPanel())) {
        printw("hidden1\n");
    }

    this->update();
    update_panels();
    doupdate();
}

bool
Window::focus() {
    PanelUserData * data = GET_PANEL_USER_DATA(getPanel());

    data->hidden = false;
    data->focused = true;

    PanelUserData * iter = panelList.begin;
    for (iter; iter; iter = iter->next) {
        if (iter->id != data->id)
            iter->focused = false;
    }

    top_panel(getPanel());
    update_panels();
    doupdate();
}

bool
Window::focused() {
    PanelUserData * data = GET_PANEL_USER_DATA(getPanel());
    return data->focused;
}

bool
Window::hidden() {
    PanelUserData * data = GET_PANEL_USER_DATA(getPanel());
    return data->hidden;
}

size_t
Window::getHeight() {
    size_t h, w;
    getmaxyx(getParent(), h, w);
    return h;
}

size_t
Window::getWidth() {
    size_t h, w;
    getmaxyx(getParent(), h, w);

    return w;
}
void
Window::getHeightAndWidth(size_t &h, size_t &w) {
    getmaxyx(getParent(), h, w);
}

bool
Window::print(int id, string content, size_t y, size_t x) {
    size_t h;
    size_t w;

    getHeightAndWidth(h, w);

    if ((x > w) || (y > h))
        return false;

    if (id < 0) {
        mvwprintw(stdscr, y, x, "%s", content.c_str());
        return true;
    }
    if (id == 0) {
        if (content.size() > w)
            return false;
        mvwprintw(getParent(), y, x,  "%s", content.c_str());
        wrefresh(getParent());
        return true;
    }

    WINDOW * widget;
    if (this->children.size() == 1){

        widget = (* this->children.begin())->child;
        mvwprintw(widget, y, x,  "%s", content.c_str());

    } else {
        /* mvwprintw(getParent(), y, x,  "%s", content.c_str()); */
        list<WindowChild *>::iterator iter;

        for (iter = this->children.begin(); iter != this->children.end(); iter++) {
            if ((*iter)->id == id) {
#ifdef DEBUG_
                wprintw (stdscr, "%d | %d\n", (*iter)->id, id);
#endif
                widget = ((* iter))->child;
                break;

            }

        }
        if (iter == this->children.end())
            return false;

        mvwprintw(widget, y, x,  "%s", content.c_str());

    }

    touchwin((WINDOW *)widget);
    this->refresh();

    return true;
}

bool
Window::clear() {
    return wclear(getParent());
}


bool
Window::clearLine(unsigned line) {
    size_t h;
    size_t w;
    bool ans;
    getmaxyx(getParent(), h, w);

    size_t i;
    for (i = 0; i < w; i++) {
        ans = mvwprintw(getParent(), line, i, "t");
        if (!ans)
            break;
    }

    ans = mvwprintw(getParent(), line, i, "%u | %u", h, w);
    update_panels();
    doupdate();

    return ans;
}


void
Window::addTitleBar(WINDOW * win, string title, size_t x) {
    if (this->type != DECORATED_WINDOW)
        return;

    size_t lx;
    size_t ly;

    getmaxyx(win, lx, ly);

    mvwaddch(win, x, 0, ACS_LTEE);
    for (size_t i = 1; i < ly-1; i++) {
        mvwaddch(win, x, i, ACS_HLINE);
    }
    mvwaddch(win, x, ly-1, ACS_RTEE);

    if (title.empty())
        return;

    this->print(0, title, 1, (ly/2)-(title.size()/2));

    this->title = title;

    update_panels();

}

bool
Window::setTitle(string title) {
    this->title = title;
    this->addTitleBar(getParent(), this->title, 2);

    update_panels();
    this->refresh();

}

bool
Window::move(size_t x, size_t y) {
    bool status;
    hide_panel(getPanel());
    status = mvwin(getParent(), x, y);
    update_panels();
    show_panel(getPanel());
    update_panels();
    doupdate();
    return status;
}

bool
Window::moveRight() {
    size_t y;
    size_t x;
    x= getbegy(this->getParent());
    y= getbegx(this->getParent());
    size_t h;
    size_t w;
    getmaxyx(getParent(), h, w);

    return this->move(x, ++y);
}

bool
Window::moveLeft() {
    size_t y;
    size_t x;
    x= getbegy(this->getParent());
    y= getbegx(this->getParent());
    size_t h;
    size_t w;
    getmaxyx(getParent(), h, w);

    return this->move(x, --y);
}

bool
Window::moveUp() {
    size_t y;
    size_t x;
    x= getbegy(this->getParent());
    y= getbegx(this->getParent());
    size_t h;
    size_t w;
    getmaxyx(getParent(), h, w);

    return this->move(--x, y);
}

bool
Window::moveDown() {
    size_t y;
    size_t x;
    x= getbegy(this->getParent());
    y= getbegx(this->getParent());
    size_t h;
    size_t w;
    getmaxyx(getParent(), h, w);

    return this->move(++x, y);
}


void *
Window::addWidget(WidgetType type, size_t h, size_t w, unsigned y, unsigned x) {
    if (h == 0 || w == 0)
        return NULL;

    WindowChild * child = new WindowChild;
    child->id = ++this->id;
    child->type = type;

    child->child = derwin(getParent(), h, w, y, x);
    if (!child->child) {
        --this->id;
        delete (child);
        return NULL;
    }
    switch (type) {
        case BUFFER_WIDGET: {
            /* todo */
            break;
        } case BORDERED_WIDGET: {
            box (child->child, 0, 0);
            /* touchwin(child->child); */
            /* this->update(); */
            break;
        }
    }

    this->children.push_back(child);

    wrefresh(getParent());
    touchwin(child->child);
    wrefresh(child->child);

    update_panels();
    doupdate();

    return child->child;
}

void*
Window::getElement(unsigned id) {
    WindowChild * c;
    list<WindowChild *>::iterator iter;
    for (iter = this->children.begin(); iter != this->children.end(); iter++){
        if ((* iter)->id == id)
            break;

    }
    if (iter == this->children.end()) {
        return NULL;

    }

    c = *iter;
    return c;

    return NULL;
}

void
Window::update() {
    if (this->hidden()) {
        /* hide_panel(getPanel()); */
        update_panels();
        doupdate();
        return;
    }

    show_panel(getPanel());
    update_panels();
    doupdate();


    /* WindowChild * c; */
    /* list<WindowChild *>::iterator iter; */
    /* for (iter = this->children.begin(); iter != this->children.end(); iter++){ */
    /*     c = (*iter); */
    /*     wrefresh(getParent()); */
    /*     touchwin(c->child); */
    /*     wrefresh(c->child); */
    /* } */
}

