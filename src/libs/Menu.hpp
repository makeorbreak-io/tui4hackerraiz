#ifndef MENU_HPP_
#define MENU_HPP_

#include <string>
#include <sstream>

#include <ncurses.h>
#include <menu.h>

#include "graphics.hpp"
#include "Window.hpp"

#define ARRAY_SIZE(a)   (sizeof(a) - sizeof(a[0]))

using namespace std;


namespace graphics {
    class Menu: public Window {
     public:
        Menu(const char ** choices, unsigned n_choices);

        Menu(const char ** choices, unsigned nchoices, size_t height,
            size_t width_, unsigned x, unsigned y, string title,
            PositionType type, string mark);

        Menu(const char ** choices, unsigned nchoices, size_t height,
            size_t width_, unsigned x, unsigned y, string title, PositionType type);

        Menu(const char ** choices, unsigned nchoices, size_t height,
             size_t width_, unsigned x, unsigned y, string title,
             const char ** descriptions);

        ~Menu();

        // getters and setters
        MENU * getParent(void);
        ITEM * getCurrentItem(void);
        WINDOW * getMenuWin(void);
        WINDOW * getSubWin(void);
        int getScale(unsigned * rows, unsigned * columns);
        string getCurrentItemName(void);
        string getMark(void);

        int setMenuWin(WINDOW * win);
        int setMenuSubWin(WINDOW * subwin);
        int setMenuSubWin(size_t h, size_t w, unsigned y, unsigned x);
        int setMenuMark(string mark);

        // misc
        int postMenu(void);
        int unpostMenu(void);
        int freeItem(ITEM * item);

        string handleKeys(void);

     private:
        MENU * parent;
        ITEM ** choices;
        ITEM * cur_item;
        unsigned nchoices;
        unsigned b;
        string mark;

    };
}

#endif /* end of include guard: MENU_HPP_ */
