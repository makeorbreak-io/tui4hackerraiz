#include "graphics.hpp"

using namespace std;

size_t graphics::lines;
size_t graphics::cols;

 bool graphics::borders;
 unsigned graphics::beg_y;
 unsigned graphics::beg_x;
 WINDOW * graphics::default_screen;

 list<void *> graphics::workspace;

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
graphics::end() {
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

int
graphics::scrolLok(WINDOW * win, bool set) {
    return scrollok(win, set);
}

int
graphics::Lines() {
    return graphics::lines;
}

int
graphics::Cols() {
    return graphics::cols;
}

int
graphics::getChar() {
    return getch();
}
