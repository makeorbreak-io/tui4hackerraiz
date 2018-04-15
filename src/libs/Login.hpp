#ifndef LOGIN_HPP_
#define LOGIN_HPP_

#include "graphics.hpp"
#include "Window.hpp"
#include "EntryText.hpp"

namespace graphics {
    class Login: public EntryText {
        public:
            Login(size_t height, size_t width_, unsigned y, unsigned x);
            ~Login();
            string * LogIn(void);

        protected:
            string ** loginData;
    };

};


#endif /* end of include guard: LOGIN_HPP_ */
