#include <string>

#include "Menu.hpp"

using namespace std;
using namespace graphics;

Menu::~Menu(void) {
    unpost_menu(getParent());
	for(unsigned i = 0; i < this->nchoices; ++i)
		free_item(choices[i]);
	free_menu(getParent());
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

Menu::Menu(const char ** choices, unsigned n_choices,
         string title, size_t h, size_t w,
         unsigned x, unsigned y, const char * item_description){

    this->setTitle(title);
    this->setHeightAndWidth(h, w);
    this->setxAndy(x, y);


    this->nchoices = n_choices;

    this->choices = (ITEM **) calloc (n_choices + 1, sizeof (ITEM *));

    for (unsigned i = 0; i < n_choices; i++) {
        this->choices[i] = new_item(choices[i], choices[i]);
    }
    this->choices[n_choices] = (ITEM *)NULL;

    this->parent = new_menu((ITEM **)this->choices);




}

MENU *
Menu::getParent(void) {
    return this->parent;
}

int
Menu::postMenu(void) {
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
    return set_menu_mark(getParent(), mark.c_str());
}

string
Menu::handleKeys(void) {
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
