#include "libs/graphics.hpp"
#include "libs/Window.hpp"
#include "libs/Menu.hpp"

#include <cstdlib>

#define MENU_OPTIONS  "/home/userx/Música/autre/Autre Ne Veut - Play By Play [Official Single].ogg", "Save", "Exit"
#define NOPTIONS  3

using namespace std;
using namespace graphics;

int
main(int argc, char **argv) {
    graphics::init();

    Window * win = new Window(15, 35, 1, 2, "teste");
    win->display();

    /* const char * choices[] = { MENU_OPTIONS }; */

    /* Menu * m = new Menu(choices, NOPTIONS, graphics::lines/2, graphics::cols, */
    /*                     (unsigned)4, (unsigned)4, string("Sei lah"), POS_CENTER, ">> "); */

    /* m->display(); */

    /* string key = m->handleKeys(); */

    /* if (!key.compare("/home/userx/Música/autre/Autre Ne Veut - Play By Play [Official Single].ogg")) { */
    /*     system("mplayer \"/home/userx/Música/autre/Autre Ne Veut - Play By Play [Official Single].ogg\" &"); */

    /* } */
    /* list<Window *> wins; */
    /* wins.push_back(new Window(15, 35, 1, 1, "teste1")); */
    /* wins.push_back(new Window(15, 35, 2, 2, "teste2")); */
    /* wins.push_back(new Window(15, 35, 3, 3, "teste3")); */
    /* wins.push_back(new Window(15, 35, 4, 4, "teste4")); */

    /* list<Window *>::iterator iterator; */
    /* list<Window *>::iterator prev; */
    /* for (prev = wins.begin(), iterator = wins.begin(); iterator != wins.end(); iterator++) { */
    /*     if (prev != iterator) { */
    /*         ((*prev))->hide(); */
    /*     } */
    /*     ((*iterator))->display(); */
    /*     graphics::getChar(); */
    /*     prev = iterator; */
    /* } */

    graphics::getChar();
    graphics::end();

    return 0;
}
