#include "Window.hpp"

#include <sstream>

using namespace std;

int
main(int argc, char **argv) {

    initscr();
    cbreak();
    /* noecho(); */

    Window * win = new Window(20, 40, 5, 5, "Title");
    /* win->display(); */

    char str[512];
    printw ("Type something: ");
    wgetnstr(stdscr, str, 512);


    curs_set(false);
    noecho();

    string content(str);
    BufferWidget * buffer = (BufferWidget *) win->addWidget(BUFFER_WIDGET, 10, 38, 2, 1);
    BufferWidget * buffer1 = (BufferWidget *) win->addWidget(BORDERED_WIDGET, 3, 38, 16, 1);

    win->print(1, content, 1, 1);

    string con;
    ostringstream os(con);
    os << "Content size: " << content.size();
    con =  os.str();
    win->print(2, con, 1, 1);

    win->display();
    char c;
    while(true) {
        c = getch();
        if (c == 'd') {
            win->moveRight();
        } else if (c == 'a') {
            win->moveLeft();
        } else  if (c == 'w') {
            win->moveUp();
        } else if (c == 's') {
            win->moveDown();
        } else if (c == 't') {
            Window * w = new Window(8, 40,  (COLS/2)-20, (LINES/2)-3);
            BorderedWidget * bordered = (BorderedWidget *) w->addWidget (BORDERED_WIDGET, 3, 36, 3, 2);
            string simple = "Type the filepath:";
            w->print(0, simple, 1, (40/2)-(simple.length()/2));

            curs_set (true);
            wmove(bordered, 1, 1);
            w->refresh(1);

            echo();

            wgetnstr(bordered, str, 512);
            noecho();

            hide_panel (w->getPanel());
            update_panels();

            con = str;
            win->print(1, con, 1, 1);
           /* w->hide(); */

            os.str("");
            os << "Content size: " << con.size();

            printw("%s\n", os.str().c_str());
            win->print(2, os.str(), 1, 1);

            curs_set(false);

            win->refresh();

        }

        if (c == 'q')
            break;

    }

    while (getch() != 'q');

    endwin();
    return 0;
}
