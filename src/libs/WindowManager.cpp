#include "graphics.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

/* #include < */

using namespace graphics;
using namespace std;

WindowManager::WindowManager(void) {

}

void
WindowManager::callLoginWindow(void) {
    EntryText * login = new EntryText(LOGIN, 12, 50,  1, 1, "");

    this->windows.push_back(login);

    login->display();
    string s = login->loginHandle();
}

void
WindowManager::callFileSelectorWindow(void) {

    EntryText * fileSelector = new EntryText(FILE_SELECTOR, 12, 50,  4, 4,  "Type a filepath");

    this->windows.push_back(fileSelector);

    fileSelector->display();
    getch();

}

void
WindowManager::callMainWindow(void) {
    int y;
    int x;

    graphics::getMaxYX(stdscr, &y, &x);

    size_t l = (70*y)/100;
    size_t c =  (70*x)/100;

    Window * win = new Window(l, c, (x/2)-(c/2), (y/2)-(l/2), "Make or Break");
    /* getmaxyx(stdscr, graphics::lines, graphics::cols); */
    /* int y; */
    /* int x; */

    /* graphics::getMaxYX(stdscr, &y, &x); */

    /* size_t l = (50*y)/100; */
    /* size_t c =  (50*x)/100; */

    /* /1* mvprintw(40, 20, "%u %u | %u %u", graphics::lines, graphics::cols, l, c); *1/ */
    /* /1* graphics::print(NULL, "%u %u | %u %u", graphics::lines, lines_, graphics::cols, cols); *1/ */

    /* getch(); */
    /* Window * win = new Window(l, c, (x/2)-(c/2), (y/2)-(l/2), "Make or Break"); */
    /* /1* PadWidget * pad = (PadWidget *) win->addWidget(PAD_WINDOW, , c/2, (l/2)-(l/4), (c/2)-(c/4)); *1/ */
    /* graphics::scrolLok(pad, true); */

    string s("\n\n\nHackathon\n\n" \
            "FIRST YOU MAKE, THEN YOU BREAK\n\n" \
            "Make or Break has a 3 day hackathon for everyone, divided into 2 main phases:\n"\
            "Make: 2 days to develop a software project (maybe add some hardware \n" \
            "Break: showcase your project to all participants in a small fair\n" \
            "Apply and experiment with a new idea to win amazing prizes! (one per member of each category’s winning team)\n\n" \
            "We\'ve got great mentors, food, and a kickass chill-out zone!)");

    /* /1* wprintw(pad, "TESTE"); *1/ */
    /* /1* pwrefresh(pad); *1/ */

    /* wprintw(win->getParent(), s.c_str()); */

    BufferWidget * buffer = (BufferWidget *) win->addWidget(BUFFER_WIDGET, l-4, c-4, 4, 2);
    /* BufferWidget * buffer = (BufferWidget *) win->addWidget(BUFFER_WIDGET, 3, 1, 10, 10); */
    graphics::scrolLok(win->getInternalWidget(1)->child, true);
    win->print(1, s.c_str());
    win->refresh();
    win->display();
}

