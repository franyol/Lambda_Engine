#include "lambda_FSM.h"
#include <iostream>

LE_StateMachine* LE_StateMachine::the_instance;

void LE_GameState::update () {
    if ( gameObjects.size() < 1 ) {
        std::cerr << "State with no game objects to update" << std::endl;
        return;
    }
    for ( auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        if ( it->second->destroy_me ) {
            delete it->second;
            it = gameObjects.erase (it);
        } else {
            it->second->update();
            it++;
        }
    }
}

void LE_GameState::render () {
    if ( gameObjects.size() < 1 ) {
        std::cerr << "State with no game objects to render" << std::endl;
        return;
    }
    for ( auto it = gameObjects.begin(); it != gameObjects.end(); ++it ) {
        it->second->render();
    }
}

void LE_StateMachine::push_back ( LE_GameState* newState ) {
    statePool.push_back ( newState );
    newState->on_enter();
}

void LE_StateMachine::pop_back () {
    statePool.back()->on_exit();
    statePool.pop_back();
}

void LE_StateMachine::update () {
    // Only updates the current state
    if (statePool.size() < 1) {
        std::cerr << "No states to update" << std::endl;
        return;
    }
    statePool.back()->update();
}

void LE_StateMachine::render () {
    // Render every state even the ones that are not updating (paused)
    if (statePool.size() < 1) {
        std::cerr << "No states to render" << std::endl;
        return;
    }
    for (auto state : statePool) {
        state->render();
    }
}

void LE_StateMachine::clean () {
    for (auto state : statePool) {
        delete state;
    }
    statePool.clear();
}
