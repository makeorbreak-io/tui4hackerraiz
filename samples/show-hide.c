#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <panel.h>

typedef struct _PANEL_DATA {
	bool hide;	/* TRUE if panel is hidden */
    bool focus;

}PANEL_DATA;

typedef struct win_ WIN;
struct win_ {
    WINDOW * win;
    size_t x;
    size_t y;
    size_t h;
    size_t w;
};

#define NLINES 10
#define NCOLS 40

void init_wins(WIN **wins, int n);
void win_show(WIN *win, char *label, int label_color);
void print_in_middle(WIN *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
    WIN *my_wins[3];
	PANEL  *my_panels[3];
	PANEL_DATA panel_datas[3];
	PANEL_DATA *temp;
	int ch;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);


	init_wins(my_wins, 3);
   	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(my_wins[0]->win); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(my_wins[1]->win); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(my_wins[2]->win); 	/* Push 2, order: stdscr-0-1-2 */

	/* Initialize panel datas saying that nothing is hidden */
	panel_datas[0].hide = FALSE;
	panel_datas[1].hide = FALSE;
	panel_datas[2].hide = FALSE;

	panel_datas[0].focus = FALSE;
	panel_datas[1].focus = FALSE;
	panel_datas[2].focus = TRUE;

	set_panel_userptr(my_panels[0], &panel_datas[0]);
	set_panel_userptr(my_panels[1], &panel_datas[1]);
	set_panel_userptr(my_panels[2], &panel_datas[2]);

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 3, 0, "Show or Hide a window with 'a'(first window)  'b'(Second Window)  'c'(Third Window)");
	mvprintw(LINES - 2, 0, "F1 to Exit");

	attroff(COLOR_PAIR(4));
	doupdate();

	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case 'a':

				temp = (PANEL_DATA *)panel_userptr(my_panels[1]);
                temp->focus = FALSE;
				temp = (PANEL_DATA *)panel_userptr(my_panels[2]);
                temp->focus = FALSE;

				temp = (PANEL_DATA *)panel_userptr(my_panels[0]);
				if(temp->hide == FALSE)
				{	hide_panel(my_panels[0]);
					temp->hide = TRUE;
                    temp->focus = FALSE;
				}
				else
				{	show_panel(my_panels[0]);
					temp->hide = FALSE;
                    temp->focus = TRUE;
				}
				break;
			case 'b':

                temp = (PANEL_DATA *)panel_userptr(my_panels[0]);
                temp->focus = FALSE;
				temp = (PANEL_DATA *)panel_userptr(my_panels[2]);
                temp->focus = FALSE;

				temp = (PANEL_DATA *)panel_userptr(my_panels[1]);
				if(temp->hide == FALSE)
				{	hide_panel(my_panels[1]);
					temp->hide = TRUE;
                    temp->focus = FALSE;
				}
				else
				{	show_panel(my_panels[1]);
					temp->hide = FALSE;
                    temp->focus = TRUE;
				}
				break;
			case 'c':

                temp = (PANEL_DATA *)panel_userptr(my_panels[0]);
                temp->focus = FALSE;
				temp = (PANEL_DATA *)panel_userptr(my_panels[1]);
                temp->focus = FALSE;

				temp = (PANEL_DATA *)panel_userptr(my_panels[2]);
				if(temp->hide == FALSE)
				{	hide_panel(my_panels[2]);
					temp->hide = TRUE;
                    temp->focus = FALSE;
				}
				else
				{	show_panel(my_panels[2]);
					temp->hide = FALSE;
                    temp->focus = TRUE;
				}
				break;
                case KEY_LEFT:
                  for (int i = 0; i < 3; i++) {
                      temp = (PANEL_DATA *) panel_userptr (my_panels[i]);
                      if (temp->focus) {
                          hide_panel (my_panels[i]);
                          mvwin (my_wins[i]->win, my_wins[i]->y, my_wins[i]->x--);
                          show_panel (my_panels[i]);
                      }

                  }
                  break;
                case KEY_RIGHT:
                  for (int i = 0; i < 3; i++) {
                      temp = (PANEL_DATA *) panel_userptr (my_panels[i]);
                      if (temp->focus) {
                          hide_panel (my_panels[i]);
                          mvwin (my_wins[i]->win, my_wins[i]->y, my_wins[i]->x++);
                          show_panel (my_panels[i]);
                      }

                  }
                  break;
                case KEY_UP:
                  for (int i = 0; i < 3; i++) {
                      temp = (PANEL_DATA *) panel_userptr (my_panels[i]);
                      if (temp->focus) {
                          hide_panel (my_panels[i]);
                          mvwin (my_wins[i]->win, my_wins[i]->y--, my_wins[i]->x);
                          show_panel (my_panels[i]);
                      }

                  }
                  break;
                case KEY_DOWN:
                  for (int i = 0; i < 3; i++) {
                      temp = (PANEL_DATA *) panel_userptr (my_panels[i]);
                      if (temp->focus) {
                          hide_panel (my_panels[i]);
                          mvwin (my_wins[i]->win, my_wins[i]->y++, my_wins[i]->x);
                          show_panel (my_panels[i]);
                      }

                  }
                  break;


		}

		update_panels();
		doupdate();
	}
	endwin();
	return 0;
}

/* Put all the windows */
void init_wins(WIN **wins, int n)
{	int x, y, i;
	char label[80];

    static char c;
    if (!c)
        c = 'A';


	y = 2;
	x = 10;
	for(i = 0; i < n; ++i)
    {
        wins[i] = (WIN *) calloc (1, sizeof (WIN));
        wins[i]->win = newwin(NLINES, NCOLS, y, x);

        wins[i]->h = NLINES;
        wins[i]->w = NCOLS;
        wins[i]->x = x;
        wins[i]->y = y;

		sprintf(label, "Window Number %d", i + 1);
        mvwprintw (wins[i]->win, 4, 4, "%c", c);
        c++;
		win_show(wins[i], label, i + 1);
		y += 3;
		x += 7;
	}
}

/* Show the window with a border and a label */
void win_show(WIN *win, char *label, int label_color)
{	int startx, starty, height, width;

	getbegyx(win->win, starty, startx);
	getmaxyx(win->win, height, width);

	box(win->win, 0, 0);
	mvwaddch(win->win, 2, 0, ACS_LTEE);
	mvwhline(win->win, 2, 1, ACS_HLINE, width - 2);
	mvwaddch(win->win, 2, width - 1, ACS_RTEE);

	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WIN *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

    if(win->win == NULL)
		win->win= stdscr;
	getyx(win->win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win->win, color);
	mvwprintw(win->win, y, x, "%s", string);
	wattroff(win->win, color);
	refresh();
}
