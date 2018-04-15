#ifndef ENTRYTEXT_HPP_
#define ENTRYTEXT_HPP_

#include "graphics.hpp"
#include "Window.hpp"

namespace graphics {
    typedef enum {
        FILE_SELECTOR,
        LOGIN,
    } EntryTextType;

    enum LoginChildOrder {
        LOGIN_TITLE = 1,
        LOGIN_LABEL,
        LOGIN_USER,
        LOGIN_PASSWORD,
    };

    class EntryText: public Window {
        public:
            EntryText(EntryTextType type, size_t height, size_t width_, unsigned y, unsigned x, string title);
            ~EntryText();

            string loginHandle(void);
            string fileSelectorHandle(void);



        protected:
            void setSubTitle (string subTitle);
            bool login_stat;


    };
};


#endif /* end of include guard: ENTRYTEXT_HPP_ */
