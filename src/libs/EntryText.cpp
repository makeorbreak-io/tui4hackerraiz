
#include "graphics.hpp"
#include "Window.hpp"
#include "EntryText.hpp"

using namespace std;
using namespace graphics;

#define LOGIN_SCREEN_TITLE "Login Screen"



EntryText::EntryText(EntryTextType type, size_t height, size_t width_, unsigned y, unsigned x, string title)
        : Window (height, width_, y, x) {

    switch(type) {
        case FILE_SELECTOR: {

            BufferWidget * label = (BufferWidget *) this->addWidget (BUFFER_WIDGET, 1, title.size()+2,
                                                                      (40*height)/100, ((50*width_)/100) - title.size()/2);

            this->print(1, "%s", title.c_str());

            BorderedWidget * entryField = (BorderedWidget *) this->addWidget(BORDERED_WIDGET, 3, (80*width_)/100,
                                                                        (60*height)/100, width_-((90*width_)/100));
            break;
        } case LOGIN: {

            string Title(LOGIN_SCREEN_TITLE);
            size_t s = Title.size();
            BufferWidget * loginTitle = (BufferWidget *) this->addWidget (BUFFER_WIDGET, 1, s,
                                                                      (15*height)/100, ((50*width_)/100) - s/2);
            this->print(LOGIN_TITLE, Title.c_str());

            BufferWidget * label = (BufferWidget *) this->addWidget (BUFFER_WIDGET, 5,  9,
                                                                     (40*height)/100, 3);
            this->print(LOGIN_LABEL, "    User:\n\n\nPassword:");

            BorderedWidget * userField = (BorderedWidget *) this->addWidget(BORDERED_WIDGET, 3, (60*width_)/100,
                                                                        (30*height)/100, ((55*width_)/100)/2);

            BorderedWidget * passField = (BorderedWidget *) this->addWidget(BORDERED_WIDGET, 3, (60*width_)/100,
                                                                        (60*height)/100, ((55*width_)/100)/2);


        }
    }

    /* graphics::hideOutput(true); */
    graphics::hideCursor(false);

    this->refresh();


}


string
EntryText::loginHandle(void) {
    char usrpwd[128];

    WindowChild * child;
    int c ;
    int i = 0;
    int times = 0;

    graphics::hideOutput(true);
    graphics::charBreak();
    graphics::keyPad(Window::getParent(), true);
    while (c = wgetch(this->getParent())) {
        if (c == 9) {
            break;
        } else if (c == 27) {
            return "";
        }
    }

    graphics::hideOutput(false);
    while (times < 2) {

        child = this->getInternalWidget((times < 1) ? LOGIN_USER : LOGIN_PASSWORD);

        c = mvwgetch(child->child, 1, 1);
        while (c != 10) {
            switch(c) {
                case KEY_BACKSPACE: {
                    i--;
                    break;
                } case 10: {
                    continue;
                    break;
                }
            }
            usrpwd[i] = c;
            i++;
            c = wgetch(child->child);
        }
        times++;
        graphics::hideOutput(true);

    }
    graphics::keyPad(Window::getParent(), false);
    graphics::hideOutput(true);
    return string(usrpwd);
}

string
EntryText::fileSelectorHandle(void) {
    char c;
    char filePath[512];
    int i = 0;
    while ((c = wgetch(this->getInternalWidget(2)->child)) != 10) {
        filePath[i++] = c;
    }
    return string(filePath);
}
