#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

#include "graphics.hpp"
#include "Window.hpp"
#include "Menu.hpp"
#include "EntryText.hpp"

#include <list>

namespace graphics {
    class WindowManager {
        public:
            WindowManager(void);
            void callLoginWindow(void);
            void callFileSelectorWindow(void);
            void callMainWindow(void);
            void callMenu(void);
            void showAll(void);

        private:
            std::list<Window *> windows;
            unsigned counter;

    };
};

#endif /* end of include guard: WINDOWMANAGER_HPP_489PUKV5f WINDOWMANAGER_HPP_ */
