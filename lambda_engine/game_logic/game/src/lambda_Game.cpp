#include "lambda_Game.h"
#include "lambda_InputHandler.h"
#include "lambda_FSM.h"
#include <iostream>

using namespace std;

LE_Game* LE_Game::the_instance;

LE_Game::LE_Game () {
                running = false;
                framerateFixed = false;
                framerate = 30;
            }

LE_Game::~LE_Game () { clean(); }


LE_Game* LE_Game::Instance() {
    if ( the_instance == nullptr ) {
        the_instance = new LE_Game();
    }
    return the_instance;
}

void LE_Game::destroyInstance() {
    if (the_instance != nullptr) {
        delete the_instance;
        the_instance = nullptr;
    }
}

double LE_Game::getDeltaTime () { return deltaTime; }

void LE_Game::fixFramerate ( int fps ) {
    framerate = fps;
    framerateFixed = true;
}

void LE_Game::unfixFramerate () { framerateFixed = false; }

Uint32 LE_Game::createWindow ( const char* title, int w, int h,
        bool full_screen, bool input_focus,
        bool hidden, bool borderless,
        bool resizable ) {
    Uint32 windowId = LE_TEXTURE->createWindow( title, w, h,
            full_screen, input_focus,
            hidden, borderless, resizable );
    windows.push_back ( windowId );
    return windowId;
}

Uint32 LE_Game::addWindow ( SDL_Window* newWindow ) {
    Uint32 windowId = LE_TEXTURE->addWindow( newWindow );
    windows.push_back ( windowId );
    return windowId;
}

Uint32 LE_Game::getWindow ( int idx ) {
    return windows.at(idx);
}

bool LE_Game::isRunning ( void ) { return running; }

void LE_Game::setRunning ( bool state ) { running = state; }

void LE_Game::handleEvents () {
    LE_INPUT->update();
}

inline void LE_Game::update () {
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
    LE_INPUT->clean();
    LE_FSM->clean();
}

void LE_Game::exit () {
    running = false;
}
