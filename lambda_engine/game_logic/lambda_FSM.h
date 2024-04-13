#ifndef _LAMBDA_ENGINE_FSM_H_
#define _LAMBDA_ENGINE_FSM_H_

    #include <vector>
    #include <map>
    #include <string>
    #include "lambda_GameObject.h"
    #include <iostream>

    /**
     * @brief shortcut for calling LE_StateMachine::Instance
     * */
    #define LE_FSM LE_StateMachine::Instance()

    /**
     * @brief shortcut for calling LE_StateMachine::the_instance destructor
     * */
    #define QUIT_LE_FSM LE_StateMachine::destroyInstance()

    /**
     * @brief base class for game states
     *
     * Each game state manages it's own game objects, logic and rendering.
     * Making wise use of LE_TextureManager, you can load only the required
     * textures the game state utilizes.
     * */
    class LE_GameState
    {
        protected:
            /**
             * @brief map containing LE_GameObjects spawned into the state
             * */
            std::map<std::string, LE_GameObject*> gameObjects;

        public:

            /**
             * @brief class constructor
             * */
            LE_GameState () {}

            /**
             * @brief class destructor
             *
             * Calls to LE_GameState::clean
             * */
            virtual ~LE_GameState () { clean(); }

            /**
             * @brief executes pre-state texture loading and game object creation
             * */
            virtual void on_enter () = 0;

            /**
             * @brief clean up all allocated memory for textures and game objects
             * used before the state exits
             * */
            virtual void on_exit () { clean(); }

            /**
             * @brief add a new game object into the state
             *
             * @param newObject object to be included
             * @param objId Game Object ID
             * */
            void addObject ( LE_GameObject* newObject, std::string objId ) {
                gameObjects[objId] = newObject;
            }

            /**
             * @brief deletes an object from the state
             *
             * @param onjId Game Object ID
             * */
            void popObject ( std::string objId ) {
                auto it = gameObjects.find(objId);
                if (it != gameObjects.end()) {
                    delete it->second;
                    gameObjects.erase(it);
                }
            }

            /**
             * @brief get object by it's ID
             *
             * @param objId Game Object ID
             * @return LE_GameObject*
             * */
            LE_GameObject* getObject ( std::string objId ) {
                auto it = gameObjects.find(objId);
                if (it != gameObjects.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /**
             * @brief updates all game Objects added to the state
             * */
            virtual void update ();

            /**
             * @brief renders all game Objects added to the state
             * */
            virtual void render ();

            /**
             * @brief deallocates all game Objects added to the state
             * */
            virtual void clean () { 
                for ( auto it = gameObjects.begin(); it != gameObjects.end(); it++ )
                    delete it->second;
                gameObjects.clear(); 
            }
    };

    /**
     * @brief Class to manage game states
     * */
    class LE_StateMachine
    {
        private:
            /**
             * @brief vector with the active LE_GameState objects
             * */
            std::vector<LE_GameState*> statePool;

            /**
             * @brief pointer containing the singleton instance
             * */
            static LE_StateMachine* the_instance;

            /**
             * @brief class constructor
             * */
            LE_StateMachine () {};
        public:

            /**
             * @brief class destructor
             *
             * calls to LE_StateMachine::clean
             * */
            ~LE_StateMachine () { clean(); }

            /**
             * @brief returns the singleton class ptr
             *
             * same as LE_FSM
             * */
            static LE_StateMachine* Instance ( void ) {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_StateMachine();
                }
                return the_instance;
            }

            /**
             * @brief destroys the class singleton
             *
             * deallocates all states
             * */
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            /**
             * @brief adds a new LE_GameState to the state machine
             * */
            void push_back ( LE_GameState* newState );

            /**
             * @brief deletes statePool back and deallocates that game state
             * */
            void pop_back  ();

            /**
             * @brief runs the LE_GameState::update method from the StatePool back
             * */
            void update ();

            /**
             * @brief renders all states in the State Pool
             * */
            void render ();

            /**
             * @brief deallocates every state in the state pool
             * */
            void clean ();
    };

#endif
