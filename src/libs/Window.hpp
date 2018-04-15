#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <curses.h>
#include <panel.h>

/* #include <cstdbool> */
#include <string>
#include <list>

using namespace std;

/*
 * **@Brief:
 * Well, this is an abstraction of ncurses's WINDOW
 * definition to include some important data to be used
 * by our CPP API.
 */

/*
 * **@Brief:
 * To ease the window management required the use of
 * the panel library, which is and extension of ncusese
 * library itself. In order to undertand what of the
 * displayed windows have the focus, we have to store a
 * auxiliary data into panels userptr space. In this case
 * I'm turning it into a simple linked list.
 */
typedef struct panel_user_data_ PanelUserData;
struct panel_user_data_ {
    int id;
    bool focused;
    bool hidden;

    PanelUserData * next;
};

typedef enum widget_type_ {
    BUFFER_WIDGET,
    BORDERED_WIDGET,
    DECORATED_WINDOW,
    MENU_WINDOW
} WidgetType;

typedef struct window_child_ WindowChild;
struct window_child_ {
    unsigned id;
    string subId;
    WidgetType type;
    WINDOW * child;
};

typedef WINDOW BufferWidget;
typedef WINDOW BorderedWidget;

namespace graphics {
    class Window {
    public:
        Window(size_t height, size_t width_, unsigned x, unsigned y, string title);
        Window(size_t height, size_t width_, unsigned x, unsigned y);   // lines, columns, begin x, begin y

        Window();                       // plots a maximized window inside the terminal
        ~Window();

        WINDOW * getParent();
        PANEL * getPanel();

        size_t getHeight();
        size_t getWidth();

        unsigned getY();
        unsigned getX();

        void getHeightAndWidth(size_t &h, size_t &w);

        bool setKeyPad(unsigned id, bool set);
        bool setTitleBar(string title);
        void setTitle(string title);
        void setPanel (PANEL &p);
        void setHeight(size_t h);
        void setWidth(size_t w);
        void setx(unsigned x);
        void sety(unsigned y);
        void setHeightAndWidth(size_t h, size_t w);
        void setxAndy(unsigned x, unsigned y);

        /* **@brief:
         * Adds a widget with h-lines and w-coluns, starting from y,x
         * (y = lines, x = columns)
         */
        void * addWidget(WidgetType type, size_t h, size_t w, unsigned y, unsigned x);

        /*
         * **@brief:
         * returns a internal widget from the window related to the ID attribute.
         */
        WindowChild * getInternalWidget(unsigned id);

        bool moveCursor(unsigned id, unsigned y, unsigned x);
        bool move(size_t x, size_t y);
        bool moveRight();
        bool moveLeft();
        bool moveUp();
        bool moveDown();

        bool refresh();
        bool display();
        bool hide();
        bool hidden();
        bool focus();
        bool focused();
        bool clear();
        bool clearLine(unsigned line);
        void refresh(unsigned id);
        int scrolLok(unsigned id, bool set);

        bool print(int id, string content, size_t y, size_t x);
        bool print(int id, unsigned y, unsigned x, const char * fmt, ...);
        bool print(int id, const char * fmt, ...);

        void update();
        unsigned id;

    protected:
        WidgetType type;
        WINDOW * parent;
        Window * title_bar;

        PANEL * panel;

        size_t decoration_padding;

        list<WindowChild *> children;

        string title; // Window Title

        size_t height;
        size_t width_;

        unsigned x;
        unsigned y;

        void addTitleBar(WINDOW * win, string title, size_t x);

    };

};

#endif /* end of include guard: _WINDOW_H_ */
