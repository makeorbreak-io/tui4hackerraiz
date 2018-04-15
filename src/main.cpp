#include "libs/graphics.hpp"
#include "libs/Window.hpp"
#include "libs/Menu.hpp"
#include "libs/EntryText.hpp"
#include "libs/WindowManager.hpp"

#include <ncurses.h>

using namespace graphics;

#define MAIN_WINDOW 0
#define BUFFERWINDOW 1


#define MENU_OPTIONS  "Open", "Save", "Exit"
#define NOPTIONS  3


int
main(int argc, char **argv) {
    graphics::init(true);

    graphics::print(NULL, "%u, %u", graphics::lines, graphics::cols);

    getch();
    WindowManager * mg = new WindowManager();

    /* mg->callLoginWindow(); */
    /* mg->callFileSelectorWindow(); */
    mg->callMainWindow();
    /* getch(); */
    /* graphics::hideOutput(true); */
    /* EntryText * entry = new EntryText(LOGIN, 12, 50,  1, 1, ""); */

    /* entry->display(); */
    /* string s = entry->loginHandle(); */

    /* graphics::hideOutput(true); */
    /* EntryText * entry = new EntryText(FILE_SELECTOR, 12, 50,  1, 1,  "Testing"); */

    /* entry->display(); */
    /* entry->moveCursor(2, 1, 1); */
    /* char c = wgetch(entry->getInternalWidget(2)->child); */
    /* graphics::print(default_screen, "%c\n", c); */



    getch();

    /* Window * win = new Window (15, 35, 1, 1, "test"); */
    /* win->addWidget(BUFFER_WIDGET, 11, 33, 3, 1); */

    /* win->setKeyPad (1, true); */
    /* win->scrolLok(1, true); */

    /* win->print(1, 1, 1, "testando\ntestando\ntestando"); */
    /* win->display(); */

    /* int i = 0; */
    /* int c; */
    /* while ((c = wgetch(win->getInternalWidget(1)->child)) != KEY_F(1)) { */
    /*     switch(c) { */
    /*         case 10: { */
    /*             wclrtobot(win->getInternalWidget(1)->child); */
    /*             win->refresh(); */
    /*             break; */
    /*         } case KEY_UP: { */
    /*             if (i > 0) { */
    /*                 i--; */
    /*                 wscrl(win->getInternalWidget(1)->child, 1); */
    /*                 win->refresh(); */
    /*             } */
    /*             break; */

    /*         } case KEY_DOWN: { */
    /*             i++; */
    /*             wscrl(win->getInternalWidget(1)->child, -1); */
    /*             win->refresh(); */
    /*             break; */
    /*         } */

    /*     } */
    /* } */
    /* hideOutput(true); */
    /* const char * choices[] = { MENU_OPTIONS }; */

    /* Menu * m = new Menu (choices, NOPTIONS, lines/3, cols/3, 2, 2, "foda-se", POS_CENTER, ">> "); */
    /* m->postMenu(); */
    /* m->display(); */

    /* Menu * me = new Menu (choices, 3, lines/3, cols/3, 5, 5, "foda-se 2", POS_CENTER); */
    /* me->postMenu(); */
    /* me->display(); */

    /* string s = m->handleKeys(); */
    /* graphics::print(NULL, "%s ", s.c_str()); */


    /* s = me->handleKeys(); */
    /* graphics::print(NULL, "%s", s.c_str()); */

    /* m->refresh(); */

    /* while ((c = getch()) != 'q') { */
    /*     switch(c) { */
    /*         case 'd': { */
    /*             m->focus(); */
    /*             break; */
    /*         } case 'D': { */
    /*             me->focus(); */
    /*             break; */
    /*         } */
    /*     } */
    /* } */

    /* getch(); */
    graphics::endWin();
    return 0;
}
