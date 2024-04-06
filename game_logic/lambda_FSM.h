#include <vector>
#include <map>
#include <string>
#include "game_object/lambda_GameObject.h"

#include <iostream>

#ifndef _LAMBDA_ENGINE_FSM_H_
#define _LAMBDA_ENGINE_FSM_H_

    #define LE_FSM LE_StateMachine::Instance()
    #define QUIT_LE_FSM LE_StateMachine::destroyInstance()

    class LE_GameState
    {
        protected:
            std::map<std::string, LE_GameObject*> gameObjects;

        public:

            LE_GameState () {}
            virtual ~LE_GameState () { clean(); }

            virtual void on_enter () = 0; // Load game objects
            virtual void on_exit () { clean(); }

            void addObject ( LE_GameObject* newObject, std::string objId ) {
                gameObjects[objId] = newObject;
            }
            void popObject ( std::string objId ) {
                auto it = gameObjects.find(objId);
                if (it != gameObjects.end()) {
                    delete it->second;
                    gameObjects.erase(it);
                }
            }
            LE_GameObject* getObject ( std::string objId ) {
                auto it = gameObjects.find(objId);
                if (it != gameObjects.end()) {
                    return it->second;
                }
                return nullptr;
            }

            virtual void update ();
            virtual void render ();
            virtual void clean () { 
                for ( auto it = gameObjects.begin(); it != gameObjects.end(); it++ )
                    delete it->second;
                gameObjects.clear(); 
            }
    };

    class LE_StateMachine
    {
        private:
            std::vector<LE_GameState*> statePool;

            // Singleton
            static LE_StateMachine* the_instance;

            LE_StateMachine () {};
        public:

            ~LE_StateMachine () { clean(); }

            static LE_StateMachine* Instance ( void ) {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_StateMachine();
                }
                return the_instance;
            }
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }


            void push_back ( LE_GameState* newState );
            void pop_back  ();

            void update ();
            void render ();
            void clean ();
    };

#endif
