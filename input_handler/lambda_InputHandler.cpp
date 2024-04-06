#include "lambda_InputHandler.h"
#include "game_logic/lambda_Game.h"
#include <iostream>

LE_InputHandler* LE_InputHandler::the_instance;

LE_InputHandler::LE_InputHandler () {}

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

            std::cout << "Initialized " << joysticks.size() << " Joystick(s)" << std::endl;
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
             * set the keystate as iddle after handling the event with set_keyState() method.
             * */
            case SDL_KEYDOWN:
                if (  event.key.repeat == 0 )
                    keys[(int)event.key.keysym.sym] = keyState::pressed;
                break;
            case SDL_KEYUP:
                if (  event.key.repeat == 0 )
                    keys[(int)event.key.keysym.sym] = keyState::released;
                break;

        }
    }
}
