#include "graphics.hpp"
#include "Window.hpp"

#include <curses.h>
#include <panel.h>

#include <cstdbool>

#define GET_PANEL_USER_DATA(obj)  (PanelUserData *) panel_userptr(obj)

using namespace graphics;

typedef struct user_data_list_ {
    int counter;
    PanelUserData * begin;
    PanelUserData * end;
} UserDataList;

static UserDataList panelList;

/* unsigned Window::id = 0; */


void
Window::new_window (size_t h, size_t w, unsigned y, unsigned x, const char * title) {
    this->height = h;
    this->width_ = w;
    if (h == 0 || w == 0)
        getmaxyx(stdscr, this->height, this->width_);

    this->parent = newwin(this->height, this->width_, 0, 0);

    this->type = (!title ? BORDERED_WIDGET : DECORATED_WINDOW);

    if (this->type == DECORATED_WINDOW) {
        this->decoration_padding = 3;
        this->addTitleBar(this->parent, title, 2);
    }

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

    this->id = 0;
    set_panel_userptr (this->panel, data);

    box(this->parent, 0, 0);

    update_panels();

    hide_panel(this->panel);
}

Window::Window(void) {
    this->new_window(0, 0, 0, 0, NULL);
}

Window::Window(size_t height, size_t width_, unsigned x, unsigned y) {
    this->new_window(height, width_, y, x, NULL);

}

Window::Window(size_t height, size_t width_, unsigned x, unsigned y, string title) {
    this->new_window (height, width_, y, x, title.c_str());

}

Window::~Window() {
    PanelUserData * prev;
    PanelUserData * temp = (PanelUserData *) panel_userptr(this->panel);
    PanelUserData * iter;
    for (iter = panelList.begin; iter; iter = iter->next) {
        if (iter->id == temp->id) {
            this->print(0, "TESTING\n");
            prev->next = iter->next;
            break;
        }
        prev = iter;
    }


    delete(((PanelUserData *) panel_userptr(getPanel())));
    del_panel(getPanel());
    delwin(getParent());
    refresh();

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

    show_panel(this->getPanel());

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
#ifdef DEBUG_
        printw("hidden1\n");
#endif

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

    this->getHeightAndWidth(h, w);


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
Window::print(int id, unsigned y, unsigned x, const char * fmt, ...) {
    if (y < this->decoration_padding)
        return false;

    WindowChild * child;
    va_list args;
    va_start(args, fmt);
    if (id < 0) {
        wmove (default_screen, y, x);
        vwprintw(default_screen, fmt, args);
    } else if (id == 0){
        wmove(this->getParent(), y, x);
        vwprintw (this->getParent(), fmt, args);
    } else {
        child = this->getInternalWidget(id);
        if (child) {
            wmove(child->child, y, x);
            vwprintw(child->child, fmt, args);
            touchwin(child->child);
        } else {
            graphics::print(NULL, "FALSE");
            return false;
        }

    }
    va_end(args);

    this->refresh();
    return true;

}


bool
Window::print(int id, const char * fmt, ...) {
    WindowChild * child;
    va_list args;
    va_start(args, fmt);
    if (id < 0) {
        vwprintw(default_screen, fmt, args);
    } else if (id == 0){
        vwprintw (this->getParent(), fmt, args);
    } else {
        child = this->getInternalWidget(id);
        if (child) {
            vwprintw(child->child, fmt, args);
            touchwin(child->child);
        } else {
            graphics::print(NULL, "FALSE");
            return false;
        }

    }
    va_end(args);

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

    /* this->print(0, 1, (ly/2)-(title.size()/2), "%s", title.c_str()); */
    this->print(0, title, 1, (ly/2)-(title.size()/2));

    this->title = title;

    update_panels();

}

void
Window::setPanel(PANEL& panel){
    this->panel = new_panel(this->parent);
}

bool
Window::setTitleBar(string title) {
    this->title = title;
    this->addTitleBar(getParent(), this->title, 2);

    update_panels();
    this->refresh();
}

void
Window::setTitle(string title){
    this->title = title;
}

void
Window::setHeight(size_t h){
    this->height = h;
}

void
Window::setWidth(size_t w){
    this->width_ = w;
}

void
Window::setHeightAndWidth(size_t h, size_t w){
    this->height = h;
    this->width_ = w;
}

void
Window::setx(unsigned x){
    this->x = x;
}

void
Window::sety(unsigned y){
    this->y = y;
}

void
Window::setxAndy(unsigned x, unsigned y){
    this->x = x;
    this->y = y;
}


bool
Window::moveCursor (unsigned id, unsigned y, unsigned x) {
    if (id < 0) {
        graphics::moveCursor(NULL, y, x);
        return true;

    } else if (id == 0) {
        graphics::moveCursor(Window::getParent(), y, x);
        return true;
    }
    WindowChild * child = this->getInternalWidget(id);
    if (!child)
        return false;
    if (!graphics::moveCursor(child->child, y, x))
        return false;

    return true;
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


    /* child->child = derwin(getParent(), h, w, y, x); */
    switch (type) {
        case BUFFER_WIDGET: {
            /* todo */
            child->child = derwin(getParent(), h, w, y, x);
            break;
        } case BORDERED_WIDGET: {
            child->child = derwin(getParent(), h, w, y, x);
            box (child->child, 0, 0);
            break;
        } case PAD_WINDOW: {
            child->child = subpad(this->getParent(),h, w, y, x);
            box(child->child, 0, 0);
        } default: {
            delete(child);
            break;
        }

    }

    if (!child || !child->child) {
        --this->id;
        delete (child);
        return NULL;
    }


    this->children.push_back(child);

    wrefresh(getParent());
    touchwin(child->child);
    wrefresh(child->child);

    update_panels();
    doupdate();

    return child->child;
}


/* void * */
/* Window::addWidget(WidgetType type, string subID, size_t h, size_t w, unsigned y, unsigned x) { */
/*     this->addWidget (type, h, w, y, x); */


/* } */

WindowChild *
Window::getInternalWidget(unsigned id) {
    WindowChild * c;
    list<WindowChild *>::iterator iter;

#ifdef DEBUG_
     printw("%lu ", this->children.size());
#endif

    for (iter = this->children.begin(); iter != this->children.end(); iter++){
        if ((* iter)->id == id)
            break;

    }

    c = *iter;
    return c;
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

int
Window::scrolLok(unsigned id, bool set) {
    WindowChild * child = this->getInternalWidget(id);
    if (!child)
        return -1;

    scrollok(child->child, set);

}

bool
Window::setKeyPad(unsigned id, bool set) {
    WindowChild * child = this->getInternalWidget(id);
    if (!child)
        return false;

    return (graphics::keyPad(child->child, set)) ? true : false;
}
/* int */
/* graphics::clear(WINDOW * win, bool redraw_borders){ */
/*         wclear(win); */
/*         if (redraw_borders) { */
/*             box(win, 0, 0); */
/*         } */
/*         doupdate(); */
/*     } */
