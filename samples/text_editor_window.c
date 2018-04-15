#include "text_editor_window.h"
#include <curses.h>

/* #define TEXT_EDITOR_WINDOW_GET_PRIVATE(o)           (o->priv) */
/* #define TEXT_EDITOR_WINDOW_GET_PARENT(o)            ((WINDOW *)o) */

/* /1* Coordinates *1/ */
/* typedef struct point_ Point; */
/* struct point_ { */
/*     size_t x; */
/*     size_t y; */
/* }; */

/* /1* Dimension definition *1/ */
/* typedef struct dimension_ Dimension; */
/* struct dimension_ { */
/*     size_t h; */
/*     size_t w; */
/* }; */

/* typedef struct text_editor_window_ TextEditorWindow; */
/* typedef struct text_editor_window_private_ TextEditorWindowPrivate; */

/* struct text_editor_window_ { */
/*     WINDOW * parent; */
/*     TextEditorWindowPrivate * priv; */

/*     Point * (* getPosition) (Point * p); */
/*     Dimension * (* getDimension) (Dimension * d); */

/*     bool (* display) (Point * p, Dimension * d); */
/* }; */

struct text_editor_window_private_ {
    unsigned int id;

    Point point;
    Dimension dimension;
};

TextEditorWindow * global_win;

TextEditorWindow *
_(TextEditorWindow * obj) {
    global_win = obj;
    return global_win;
}

Point
text_editor_get_position (void) {
    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE(global_win);

    return priv->point;
}

Dimension
text_editor_get_dimension (void) {
    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE(global_win);

    return priv->dimension;
}

void
text_editor_set_position (size_t x, size_t y) {
    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE (global_win);
    priv->point.x = x;
    priv->point.y = y;
}

void
text_editor_set_dimension (size_t h, size_t w) {
    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE (global_win);

    priv->dimension.h = h;
    priv->dimension.w = w;
}

bool
text_editor_display_at (Point p) {
    text_editor_set_position (p.x, p.y);

    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE (global_win);

    global_win->parent = newwin (priv->dimension.h, priv->dimension.w,
                                priv->point.x, priv->point.y);

    box (global_win->parent, 0, 0);
    if (global_win->parent)
        return true;
    return false;

}

bool
text_editor_display (void) {

    TextEditorWindowPrivate * priv = TEXT_EDITOR_WINDOW_GET_PRIVATE (global_win);

    if (!global_win->parent)
        global_win->parent = newwin (priv->dimension.h, priv->dimension.w,
                                    priv->point.x, priv->point.y);


    box (global_win->parent, 0, 0);
    if (global_win->parent)
        return true;
    return false;

}

bool
text_editor_window_wrefresh (void) {
    wrefresh(global_win->parent);
    return true;
}

TextEditorWindow *
TextEditorWindowNew (size_t x, size_t y, size_t h, size_t w) {
    TextEditorWindow * win = (TextEditorWindow *) calloc (1, sizeof(TextEditorWindow));
    win->priv = (TextEditorWindowPrivate *) calloc (1, sizeof (TextEditorWindowPrivate));

    win->getPosition =  text_editor_get_position;
    win->setDimension  = text_editor_set_position;

    win->getDimension = text_editor_get_dimension;
    win->setDimension = text_editor_set_dimension;

    win->display_at = text_editor_display_at;
    win->display = text_editor_display;
    win->wrefresh = text_editor_window_wrefresh;

    win->priv->point.x = x;
    win->priv->point.y = y;
    win->priv->dimension.h = h;
    win->priv->dimension.w = w;

    return win;
}

