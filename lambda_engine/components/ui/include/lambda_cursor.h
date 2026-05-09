#ifndef _LAMBDA_CURSOR_H_
#define _LAMBDA_CURSOR_H_

#include "lambda_GameObject.h"
#include "lambda_InputHandler.h"
#include <SDL2/SDL_stdinc.h>
#include <string>
enum class cursorState {
    clicked,
    hover,
    iddle
};

class LE_Cursor: public LE_GameObject {
    public:
        std::string variant = "black";
        cursorState state = cursorState::iddle;

        LE_Cursor ();

        LE_Cursor (std::string variant);

        ~LE_Cursor ();

        void setup () override;

        void update () override;

        void on_hover ();

        void on_click ();

        void on_idle ();

        static void init(Uint32 windowId);
};

#endif
