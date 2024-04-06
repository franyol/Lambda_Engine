#include "lambda_Game.h"
#include "input_handler/lambda_InputHandler.h"
#include "lambda_FSM.h"
#include <iostream>

using namespace std;

LE_Game* LE_Game::the_instance;

void LE_Game::handleEvents () {
    LE_InputHandler::Instance()->update();
}

void LE_Game::update () {
    LE_StateMachine::Instance()->update();
}

void LE_Game::render () {
    for ( Uint32 windowId : windows ) {
        LE_TEXTURE->fillBackground( windowId, 0, 0, 0, 0 );
    }

    LE_FSM->render();

    for ( Uint32 windowId : windows ) {
        LE_TEXTURE->present( windowId );
    }
}

void LE_Game::mainLoop () {

    if (!LE_TEXTURE->EverythingWasInit()) {
        std::cerr << "Could not init texture manager" << std::endl;
        return;
    }

    Uint32 frameStart, frameTime;

    running = true;
    while ( running ) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if ( framerateFixed ) {
            // Fix framerate
            double frameMaxTime = 1000.0f/framerate;
            if ( frameTime < frameMaxTime ) {
                SDL_Delay( (int)(frameMaxTime - frameTime) );
                deltaTime = frameMaxTime;
            } else {
                deltaTime = frameTime;
            }
        } else {
            deltaTime = frameTime;
        }
    }
    clean();
}

void LE_Game::clean () {
    LE_TEXTURE->clean();
    LE_InputHandler::Instance()->clean();
}

void LE_Game::exit () {
    cout << "Cleaning up" << endl;
    running = false;
    clean();
}
