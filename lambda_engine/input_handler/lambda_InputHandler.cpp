#include "lambda_InputHandler.h"
#include "lambda_Game.h"
#include <iostream>

LE_InputHandler* LE_InputHandler::the_instance;

LE_InputHandler::LE_InputHandler () {
    mouse.left = keyState::iddle;
    mouse.middle = keyState::iddle;
    mouse.right = keyState::iddle;
}

void LE_InputHandler::initJoysticks () {
    if ( SDL_WasInit(SDL_INIT_JOYSTICK) == 0 ) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    if ( SDL_NumJoysticks() > 0) {
        for ( int i = 0; i < SDL_NumJoysticks(); i++ ) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if ( joy != NULL ) {
                joysticks.push_back(joy);
            } else {
                std::cerr << "Error opening joystick " << i << ": " 
                    << SDL_GetError() << std::endl;
            }
            SDL_JoystickEventState(SDL_ENABLE);
            is_joysticks_initialized = true;

            std::cout << "Initialized " << joysticks.size()
                << " Joystick(s)" << std::endl;
        }
    } else {
        is_joysticks_initialized = false;
    }
}

void LE_InputHandler::clean() {
    if ( is_joysticks_initialized ) {
        for ( unsigned int i = 0; i < SDL_NumJoysticks(); i++ ) {
            SDL_JoystickClose(joysticks[i]);
        }
    }
}

void LE_InputHandler::update() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
            case SDL_QUIT: 
                LE_Game::Instance()->exit();
                break;

            /*
             * Note: keyState can be pressed or released, it starts in iddle by default
             * in case you want to manage actions when a button is released. In that case,
             * set the keystate as iddle after handling the event with set_keyState() 
             * method.
             * */
            case SDL_KEYDOWN:
                if (  event.key.repeat == 0 )
                    keys[(int)event.key.keysym.sym] = keyState::pressed;
                break;
            case SDL_KEYUP:
                if (  event.key.repeat == 0 )
                    keys[(int)event.key.keysym.sym] = keyState::released;
                break;

            // Mouse events
            case SDL_MOUSEWHEEL:
                mouse.windowId = event.wheel.windowID;
                mouse.scrollx = event.wheel.preciseX;
                mouse.scrolly = event.wheel.preciseY;
                mouse.xabs = event.wheel.mouseX;
                mouse.yabs = event.wheel.mouseY;
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                mouse.xabs = event.button.x;
                mouse.yabs = event.button.y;
                switch ( event.button.button ) {
                    case SDL_BUTTON_LEFT:
                        if ( event.button.state == SDL_PRESSED )
                            mouse.left = keyState::pressed;
                        else 
                            mouse.left = keyState::released;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        if ( event.button.state == SDL_PRESSED )
                            mouse.middle = keyState::pressed;
                        else 
                            mouse.middle = keyState::released;
                        break;
                    case SDL_BUTTON_RIGHT:
                        if ( event.button.state == SDL_PRESSED )
                            mouse.right = keyState::pressed;
                        else 
                            mouse.right = keyState::released;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                mouse.was_moved = true;
                SDL_GetMouseState(&mouse.xabs, &mouse.yabs);
                mouse.xrel = event.motion.xrel;
                mouse.yrel = event.motion.yrel;
                break;
            // TODO: add game controllers and gather joystick info

        }
    }
}
