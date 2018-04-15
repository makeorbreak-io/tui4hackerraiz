#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <stdarg.h>

#include <ncurses.h>
#include <panel.h>
#include <menu.h>

#define COL_CENTER(cols)          ((graphics::cols/2)-((cols)/2))
#define LIN_CENTER(lins)          ((graphics::lines/2)- ((lins)/2))

namespace graphics {
    static WINDOW * init();
    static WINDOW * init(bool border);

    static int hideOutput(bool set);
    static int hideCursor(bool set);

    static int charBreak(void);
    static int endWin(void);
    static int clear(void);
    static int hidePanel(PANEL * panel);
    static int doUpdate(void);
    static int refresh(void);
    static int keyPad(WINDOW * win, bool set);
    static int moveCursor(WINDOW * win, unsigned y, unsigned x);
    static int print(WINDOW * w, const char * fmt, ...);
    static int getMaxYX(WINDOW *win, int * y, int * x);


    static bool borders;

    static int lines = 0;
    static int cols = 0;


    /* int lines; */
    /* int cols; */
    static unsigned beg_y;
    static unsigned beg_x;
    static WINDOW * default_screen;

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


WINDOW *
graphics::init() {
    graphics::default_screen = initscr();
    graphics::beg_y = 0;
    graphics::beg_x = 0;

    getmaxyx(stdscr, graphics::lines, graphics::cols);
    graphics::borders = false;
}

WINDOW *
graphics::init(bool border) {
    graphics::default_screen = initscr();
    graphics::beg_y = 0;
    graphics::beg_x = 0;

    getmaxyx(default_screen, graphics::lines, graphics::cols);
    if (border) {
        box(graphics::default_screen, 0, 0);
        graphics::borders = true;
    }

}

int
graphics::charBreak() {
    return cbreak();
}

int
graphics::hideOutput(bool set) {
    if (set)
        return noecho();
    return echo();

}

int
graphics::endWin() {
    endwin();
}

int
graphics::clear() {
    if (graphics::borders) {
        box(graphics::default_screen, 0, 0);
    }
}

int
graphics::hidePanel(PANEL * p) {
    hide_panel (p);
}

int
graphics::doUpdate(void) {
    doupdate();
}

int
graphics::moveCursor(WINDOW * win, unsigned int y, unsigned int x) {
    if (!win)
        return move(y, x);
    return wmove (win, y, x);
}

int
graphics::print(WINDOW * w, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (w)
        vwprintw(w, fmt, args);
    else
        vwprintw (default_screen, fmt, args);

    va_end(args);
}

int
graphics::hideCursor(bool set) {
    curs_set(!set);
}


int
graphics::keyPad(WINDOW * win, bool set) {
    if (!win) {
        return 0;
    }
    return keypad(win, set);
}

int
graphics::getMaxYX(WINDOW * win, int * y, int * x) {
    return getmaxyx(win, *y, *x);
}

#endif /* end of include guard: GRAPHICS_HPP_ */
