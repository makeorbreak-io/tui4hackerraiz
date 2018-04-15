#ifndef MENU_HPP_
#define MENU_HPP_

#include <string>
#include <sstream>

#include <ncurses.h>
#include <menu.h>

#include "graphics.hpp"

#define ARRAY_SIZE(a)   (sizeof(a) - sizeof(a[0]))

using namespace std;

class Menu: public Window {
 public:
    Menu(const char ** choices, unsigned n_choices);
    Menu(const char ** choices, unsigned n_choices,
         string title, size_t h, size_t w,
         unsigned x, unsigned y, const char * item_description);
    ~Menu();

    // getters and setters
    MENU * getParent(void);
    ITEM * getCurrentItem(void);
    string getCurrentItemName(void);
    WINDOW * getMenuWin(void);
    WINDOW * getSubWin(void);
    int getScale(unsigned * rows, unsigned * columns);

    int setMenuWin(WINDOW * win);
    int setMenuSubWin(WINDOW * subwin);
    int setMenuSubWin(size_t h, size_t w, unsigned y, unsigned x);
    int setMenuMark(string mark);

    // misc
    int postMenu(void);
    int unpostMenu(void);
    string handleKeys(void);

 private:
  MENU * parent;
  ITEM ** choices;
  ITEM * cur_item;
  unsigned nchoices;
  unsigned b;

};

#endif /* end of include guard: MENU_HPP_ */
