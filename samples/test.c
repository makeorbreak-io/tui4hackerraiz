#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <sys/ioctl.h>

#define VERTICAL_BORDER             '|'
#define HORIZONTAL_BORDER           '-'
#define CORNER                      '+'
#define set_cursor_pos(x,y)         printf("\033[%d;%dH", (x), (y))

unsigned short lines;
unsigned short cols;

void
get_term_size(unsigned short * height, unsigned short * width) {
    struct winsize ws = {0, 0, 0, 0};
    if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
        exit(EXIT_FAILURE);
    }

    *height = ws.ws_row;
    *width = ws.ws_col;
}

void __attribute__((constructor))
set_current_term_size (void) {
    get_term_size (&lines, &cols);
}

void
draw_window (unsigned short h, unsigned short w) {
    unsigned short i;
    unsigned short j;

    /* h = h / 2; */
    /* w = w/2; */
    for (i = 0; i < h-1; i++) {
        for (j = 0; j < w; j++) {
            if (j == 0 || j == w-1) {
                if (i == 0 || i == h-2) {
                    printf ("+");
                    continue;
                }
                printf ("|");
            } else {
                if (i == 0 || i == h-2) {
                    printf ("-");
                    continue;
                }
                printf (" ");
            }
        }
        printf ("\n");
    }
}

void
window_print_text (const char * content, unsigned short h, unsigned short w, int x, int y) {
    if (x >= h || y >= w)
        return;

    draw_window (h, w);
    set_cursor_pos (x, y);

    int  relative_y_pos;
    relative_y_pos = (w)-y;
    /* printf ("relative: %d\n", relative_y_pos); */
    if (relative_y_pos < 1) {
        set_cursor_pos(x+1, 2);
        relative_y_pos = (w) - 2;

    } else {
        int i = 0;
        int j = 0;
        while (i < strlen (content)) {
            for (i = j; i < relative_y_pos  && i < strlen(content); i++) {
                putchar (content[i]);
                relative_y_pos--;
                j++;
            }
            if (i < strlen (content)) {
                set_cursor_pos(x+1, 2);
                relative_y_pos = (w)-2;
            }
        }
    }

}

int
main(int argc, char **argv) {
    unsigned short h;
    unsigned short w;
    get_term_size (&h, &w);
    /* printf ("Window Size:\n\tHeight: %d\n\tWidth: %d\n", h, w); */
    /* draw_window (h, w); */
    window_print_text ("Não existe pátria para quem desespera e, quanto a mim, sei que o mar me precede e me segue, e minha loucura está sempre pronta. Aqueles que se amam e são separados podem viver sua dor, mas isso não é desespero: eles sabem que o amor existe. Eis porque sofro, de olhos secos, este exílio. Espero ainda. Um dia chega, enfim... ", h, w, h/2, w-3);
    /* window_print_text ("victor", h, w, h/2, (w/2)-(strlen("victor")/2)); */
    set_cursor_pos (lines, 0);
    return 0;
}
