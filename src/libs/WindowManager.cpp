#include "graphics.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

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

    /* getmaxyx(stdscr, graphics::lines, graphics::cols); */
    int y;
    int x;

    /* graphics::getMaxYX(stdscr, &y, &x); */

    size_t l = ((50*graphics::lines)/100);
    size_t c =  (50*graphics::cols)/100;

    mvprintw(40, 20, "%u %u | %u %u", graphics::lines, graphics::cols, l, c);
    /* graphics::print(NULL, "%u %u | %u %u", graphics::lines, lines_, graphics::cols, cols); */

    getch();
    Window * win = new Window(15, 30, 7, 7, "Make or Break");

    win->display();
}

