#include "libs/Window.hpp"
#include "libs/Menu.hpp"

#include <sstream>

#include <ncurses.h>

using namespace std;
using namespace graphics;

#define MAINWINDOW_COLS   40
#define MAINWINDOW_LINS    4

using namespace graphics;

int
main(int argc, char **argv) {

    initscr();

    cbreak();
    noecho();
    curs_set(false);

    box (stdscr, 0, 0);
    size_t gh;
    size_t gw;
    getmaxyx(stdscr, gh, gw);

    size_t cols = (gw/2) - (MAINWINDOW_COLS/2);
    size_t lines = (gh/2) - (MAINWINDOW_LINS/2);

    Window * contentWin;
    BufferWidget * contentBuffer;

    Window * win = new Window(7, 40, cols, lines);
    BorderedWidget * border = (BorderedWidget *)win->addWidget(BORDERED_WIDGET, 3, 36, 3, 2);
    win->print(0, "Type something:", 2, (win->getWidth()/2)-(string("Type something:").size()/2));

    win->print(-1, "'w' to focus on the text entry widget", 1, 1);
    win->print(-1, "'d' to minimize and restore the window", 2, 1);

    scrollok(border, true);
    immedok(border, true);
    wmove(border, 1, 1);

    win->refresh();

    win->display();
    string content;
    bool flow;
    int c = 0x00;
    while(c != 'q') {
        c = getch();
        switch(c) {
            case 'd':
                if (win->hidden()) {
                    win->display();
                } else {
                    win->hide();
                }
                break;
            case 'w':
                if (win->hidden()) {
                    win->display();
                    wmove(border, 1, 1);
                }
                echo();
                curs_set(true);
                char str[512] = {0x00};
                wgetnstr(border, str, 512);
                content = str;
                win->hide();

                size_t cc = (COLS/2) - (40/2);
                size_t cl = (LINES/2) - (20/2);
                contentWin = new Window (20, 40, cc, cl, "Content typed");
                contentWin->display();

                contentBuffer = (BufferWidget *)contentWin->addWidget (BUFFER_WIDGET, 10, 38, 2, 1);
                contentWin->print(1, content, 1, 1);

                BorderedWidget * contentBorder = (BorderedWidget *)contentWin->addWidget (BORDERED_WIDGET, 3, 38, 16, 1);
                ostringstream os;
                os << "Content size: " << content.size();

                string info = os.str();
                contentWin->print(2, info, 1, 1);
                c = 'q';
                noecho();
                curs_set(false);
                break;

        }
    }

    win->print(-1, "'w,a,s,d' to move the window", 1, 1);
    win->print(-1, "'q' to close it", 2, 1);
    win->print(-1, "'h' to to minimize and restore", 3, 1);


    while ((c = getch()) != 'q') {
        switch(c) {
            case 'w':
                contentWin->moveUp();
                break;
            case 's':
                contentWin->moveDown();
                break;
            case 'a':
                contentWin->moveLeft();
                break;
            case 'd':
                contentWin->moveRight();
                break;
            case 'h':
                if (contentWin->hidden()) {
                    contentWin->display();
                } else {
                    contentWin->hide();
                }
                break;
            /* case 'c': */
            /*     BorderedWidget * contentBorder = (BorderedWidget *)contentWin->addWidget (BORDERED_WIDGET, 3, 38, 16, 1); */
            /*     contentWin->print(2, content, 1, 1); */

        }
    }
    win->hide();
    contentWin->hide();

    graphics::clear(stdscr, true);

    const char * menu_items[] = { "Entry Text Window", "General Window", "Exit"};
    Menu * menu = new Menu(menu_items, 3);
    unsigned l, cl;
    menu->getScale(&l, &cl);
    l = l*4;
    Window * menu_win = new Window(l+4, cl+4, cols, lines, "Menu Window");

    menu->setMenuWin(menu_win->getParent());
    BufferWidget * menuB = (BufferWidget *)menu_win->addWidget(BUFFER_WIDGET, l, cl, 3, 1);
    menu->setMenuSubWin (menuB);
    menu->setMenuMark(" > ");
    menu->postMenu();
    menu_win->display();

    string s = menu->handleKeys();

    menu->unpostMenu();
    menu_win->hide();

    if (!s.compare("Exit")){
        endwin();
        return 0;
    }

    printw ("%s\n", s.c_str());
    getch();
    endwin();
    return 0;
}
