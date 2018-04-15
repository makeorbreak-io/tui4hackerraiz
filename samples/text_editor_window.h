#ifndef TEXT_EDITOR_WINDOW_H_
#define TEXT_EDITOR_WINDOW_H_

#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEXT_EDITOR_WINDOW_GET_PRIVATE(o)           (o->priv)
#define TEXT_EDITOR_WINDOW_GET_PARENT(o)            ((WINDOW *)o)

/* Coordinates */
typedef struct point_ Point;
struct point_ {
    size_t x;
    size_t y;
};

/* Dimension definition */
typedef struct dimension_ Dimension;
struct dimension_ {
    size_t h;
    size_t w;
};

typedef struct text_editor_window_ TextEditorWindow;
typedef struct text_editor_window_private_ TextEditorWindowPrivate;

struct text_editor_window_ {
    WINDOW * parent;
    TextEditorWindowPrivate * priv;

    Point (* getPosition) (void);
    Dimension (* getDimension) (void);

    void (* setPostion) (size_t x, size_t);
    void (* setDimension)(size_t h, size_t w);

    bool (* wrefresh) (void);
    bool (* display_at) (Point p);
    bool (* display) (void);
};

TextEditorWindow * _(TextEditorWindow * obj);
TextEditorWindow * TextEditorWindowNew (size_t x, size_t y, size_t h, size_t w);

#endif /* end of include guard: TEXT_EDITOR_WINDOW_H_ */
