#include "lambda_FSM.h"

LE_StateMachine* LE_StateMachine::the_instance;


LE_GameState::LE_GameState () {}

void LE_GameState::addObject ( LE_GameObject* newObject, std::string objId ) {
    objectQueue.push_back( { newObject, objId } );
}

void LE_GameState::popObject ( std::string objId ) {
    objectDeleteQueue.push_back( objId );
}

LE_GameObject* LE_GameState::getObject ( std::string objId ) {
    auto it = gameObjects.find(objId);
    if (it != gameObjects.end()) {
        return it->second;
    }
    return nullptr;
}

inline void LE_GameState::update () {
    if (objectQueue.size() > 0) {
        for (int i = 0; i < objectQueue.size(); i++) {
            gameObjects[objectQueue[i].objID] = objectQueue[i].newObject;
            objectQueue[i].newObject->setup();
        }
        objectQueue.clear();
    }
    if (objectDeleteQueue.size() > 0) {
        for (int i = 0; i < objectDeleteQueue.size(); i++) {
            auto it = gameObjects.find ( objectDeleteQueue[i] );
            if ( it != gameObjects.end() ) {
                delete it->second;
                gameObjects.erase(it);
            }
        }
        objectDeleteQueue.clear();
    }
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


LE_StateMachine::LE_StateMachine () {};

LE_StateMachine::~LE_StateMachine () { clean(); }

LE_StateMachine* LE_StateMachine::Instance ( void ) {
    if ( the_instance == nullptr ) {
        the_instance = new LE_StateMachine();
    }
    return the_instance;
}

LE_GameState* LE_StateMachine::getCurrentState () {
    if (statePool.size() < 1) return nullptr;
    return statePool.back();
}

void LE_StateMachine::destroyInstance () {
    if ( the_instance != nullptr ) {
        delete the_instance;
        the_instance = nullptr;
    }
}

void LE_StateMachine::addGenerator ( std::string stateId, LE_GameState* (*func)() ) {
    auto it = stateGenerators.find ( stateId );
    if ( it != stateGenerators.end() ) {
        std::cout << stateId << " Is already mapped to a generator";
        return;
    }

    stateGenerators[ stateId ] = func;
}

void LE_StateMachine::popGenerator ( std::string stateId ) {
    stateGenerators.erase ( stateId );
}

void LE_StateMachine::push_back ( LE_GameState* newState ) {
    changeQueue.push_back( newState );
}

void LE_StateMachine::push_back ( std::string stateId ) {
    auto it = stateGenerators.find ( stateId );
    if ( it == stateGenerators.end() ) {
        std::cout << "Error pushing state " << stateId <<
            ": generator not found";
        return;
    }
    // Call generator
    push_back (it->second());
}

void LE_StateMachine::pop_back () {
    changeQueue.push_back( nullptr );
}

void LE_StateMachine::update () {

    // Check for state changes
    if (changeQueue.size() > 0) {
        for (int i = 0; i < changeQueue.size(); i++) {
            if ( changeQueue[i] == nullptr ) {
                statePool.back()->on_exit();
                delete statePool.back();
                statePool.pop_back();
            } else {
                statePool.push_back ( changeQueue[i] );
                changeQueue[i]->on_enter();
            }
        }
        changeQueue.clear();
    }

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
    changeQueue.clear();
    stateGenerators.clear();
}
