#include <stdio.h>
#include <curses.h>

#include "text_editor_window.h"

int
main(int argc, char **argv) {

    initscr();
    cbreak();
    noecho();

    TextEditorWindow * win = TextEditorWindowNew (2, 2, 50, 100);

    _(win)->display();

    TextEditorWindow * winn = TextEditorWindowNew (10, 10, 50, 50);

    _(winn)->display();


    refresh ();
    _(win)->wrefresh();
    _(winn)->wrefresh();

    wmove(win->parent, 2, 2);
    refresh();
    _(win)->wrefresh();


    int c;
    while ((c = getch ()) != 'q');
    endwin ();


    return 0;
}
