#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <stdarg.h>

#include <list>

#include <ncurses.h>
#include <panel.h>
#include <menu.h>

#define COL_CENTER(cols)          ((graphics::cols/2)-((cols)/2))
#define LIN_CENTER(lins)          ((graphics::lines/2)- ((lins)/2))

namespace graphics {
    WINDOW * init();
    WINDOW * init(bool border);

    int hideOutput(bool set);
    int hideCursor(bool set);

    int charBreak(void);
    int end(void);
    int clear(void);
    int hidePanel(PANEL * panel);
    int doUpdate(void);
    int refresh(void);
    int keyPad(WINDOW * win, bool set);
    int moveCursor(WINDOW * win, unsigned y, unsigned x);
    int print(WINDOW * w, const char * fmt, ...);
    int getMaxYX(WINDOW *win, int * y, int * x);
    int scrolLok(WINDOW * win, bool set);
    int Lines();
    int Cols();
    int getChar();

    extern size_t lines;
    extern size_t cols;
    extern bool borders;
    extern unsigned beg_y;
    extern unsigned beg_x;
    extern WINDOW * default_screen;

    extern std::list<void *> workspace;

    typedef enum positon_type_ {
        POS_UP_LEFT,
        POS_UP_RIGHT,
        POS_UP_CENTER,
        POS_CENTER_LEFT,
        POS_CENTER_RIGHT,
        POS_CENTER,
        POS_DOWN_LEFT,
        POS_DOWN_RIGHT,
        POS_DOWN_CENTER,
    } PositionType;

};

#endif /* end of include guard: GRAPHICS_HPP_ */
