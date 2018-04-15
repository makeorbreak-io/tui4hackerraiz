#include <string>

#include "Menu.hpp"
#include "Window.hpp"

using namespace std;
using namespace graphics;

Menu::~Menu(void) {
    this->unpostMenu();
	for(unsigned i = 0; i < this->nchoices; ++i)
	    this->freeItem(choices[i]);
	free_menu(this->parent);

    graphics::print(default_screen, "chegou\n\n\n");
}

Menu::Menu(const char ** choices, unsigned n_choices) {
    this->nchoices = n_choices;

    this->choices = (ITEM **) calloc (n_choices + 1, sizeof (ITEM *));

    for (unsigned i = 0; i < n_choices; i++) {
        this->choices[i] = new_item(choices[i], choices[i]);
    }
    this->choices[n_choices] = (ITEM *)NULL;

    this->parent = new_menu((ITEM **)this->choices);

}



Menu::Menu(const char ** choices, unsigned nchoices, size_t height,
    size_t width_, unsigned x, unsigned y, string title,
    PositionType type, string mark) : Window(height, width_, x, y, title) {

    this->nchoices = nchoices;
    this->choices = (ITEM **) calloc (nchoices + 1, sizeof (ITEM *));

    for (unsigned i = 0; i < nchoices; i++) {
        this->choices[i] = new_item(choices[i], NULL);
    }
    this->choices[nchoices] = (ITEM *)NULL;

    this->parent = new_menu((ITEM **)this->choices);
    setMenuWin (Window::getParent());
    this->setMenuMark(mark);

    unsigned rows, cols;
    this->getScale(&rows, &cols);

    BufferWidget * child = NULL;
    switch (type) {
        case POS_CENTER: {
            child = (BufferWidget *)addWidget(BUFFER_WIDGET,
                                             rows+4, cols+8, (height/2)-(rows/2),
                                             (width_/2) - (cols/2) - mark.size());
        }
    }
    if (child) {
        setMenuSubWin(child);
    }
}



Menu::Menu(const char ** choices, unsigned nchoices, size_t height,
    size_t width_, unsigned x, unsigned y, string title, PositionType type)
    : Window(height, width_, x, y, title) {

    this->nchoices = nchoices;
    this->choices = (ITEM **) calloc (nchoices + 1, sizeof (ITEM *));

    for (unsigned i = 0; i < nchoices; i++) {
        this->choices[i] = new_item(choices[i], NULL);
    }
    this->choices[nchoices] = (ITEM *)NULL;

    this->parent = new_menu((ITEM **)this->choices);
    setMenuWin (Window::getParent());

    unsigned rows, cols;
    this->getScale(&rows, &cols);
    this->setMenuMark("");

    BufferWidget * child = NULL;
    switch (type) {
        case POS_CENTER: {
            child = (BufferWidget *)addWidget(BUFFER_WIDGET,
                                             rows+4, cols+8, (height/2)-(rows/2),
                                             (width_/2) - (cols/2));
        }
    }
    if (child) {
        setMenuSubWin(child);
    }
}


Menu::Menu(const char ** choices, unsigned nchoices, size_t height,
             size_t width_, unsigned x, unsigned y, string title,
             const char ** descriptions) : Window(height, width_, x, y, title) {

    this->nchoices = nchoices;
    this->choices = (ITEM **) calloc (nchoices + 1, sizeof (ITEM *));

    for (unsigned i = 0; i < nchoices; i++) {
        this->choices[i] = new_item(choices[i], descriptions[i]);
    }

    this->choices[nchoices] = (ITEM *)NULL;

    this->parent = new_menu((ITEM **)this->choices);
    setMenuWin (Window::getParent());

    unsigned rows, cols;
    this->getScale(&rows, &cols);
    this->setMenuMark("");

    BufferWidget * child = NULL;
    switch (type) {
        case POS_CENTER: {
            child = (BufferWidget *)addWidget(BUFFER_WIDGET,
                                             rows+4, cols+8, (height/2)-(rows/2),
                                             (width_/2) - (cols/2));
        }
    }
    if (child) {
        setMenuSubWin(child);
    }
}


int
Menu::freeItem(ITEM * item) {
    return free_item(item);
}

MENU *
Menu::getParent(void) {
    return this->parent;
}

int
Menu::postMenu(void) {
    /* graphics::hideOutput(true); */
    graphics::hideCursor(true);
    return post_menu(getParent());
}

int
Menu::unpostMenu(void) {
    return unpost_menu(getParent());
}
// getters
WINDOW *
Menu::getMenuWin(void) {
    return menu_win(getParent());
}

WINDOW *
Menu::getSubWin(void) {
    return menu_sub(getParent());
}

int
Menu::getScale(unsigned * rows, unsigned * columns) {
    return scale_menu(getParent(), (int *)rows, (int *)columns);

}

ITEM *
Menu::getCurrentItem(void) {
    return current_item(getParent());
}

string
Menu::getCurrentItemName(void) {
    return string(item_name(current_item(getParent())));
}

// setters
int
Menu::setMenuWin(WINDOW * win) {
    return set_menu_win(this->getParent(), win);
}

int
Menu::setMenuSubWin(WINDOW * subwin) {
    return set_menu_sub(getParent(), subwin);
}

int
Menu::setMenuSubWin(size_t h, size_t w, unsigned y, unsigned x) {
    return set_menu_sub (getParent(), derwin(getMenuWin(), h, w, y, x));
}

int
Menu::setMenuMark(string mark) {
    this->mark = mark;
    return set_menu_mark(getParent(), mark.c_str());
}

string
Menu::getMark(void) {
    return this->mark;
}

string
Menu::handleKeys(void) {
    this->refresh();
    keypad(getMenuWin(), true);
    wrefresh(getSubWin());
    int c;
    while ((c = wgetch(getMenuWin()))) {
        switch (c) {
            case KEY_DOWN: {
                menu_driver (getParent(), REQ_DOWN_ITEM);
                break;
            } case KEY_UP: {
                menu_driver (getParent(), REQ_UP_ITEM);
                break;
            } case 10: {
                string s = getCurrentItemName();
                keypad(getMenuWin(), false);
                return s;
                break;
            }
        }

    }
}
