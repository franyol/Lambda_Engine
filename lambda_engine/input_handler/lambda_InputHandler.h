#include <SDL2/SDL.h>
#include <vector>
#include <map>

#ifndef _LAMBDA_INPUT_HANDLER_H_
#define _LAMBDA_INPUT_HANDLER_H_

    /**
     * @brief shortcut for getting LE_InputHandler::the_instance ptr
     * */
    #define LE_INPUT LE_InputHandler::Instance()

    /**
     * @brief shortcut for deallocation LE_InputHandler::the_instance
     * */
    #define QUIT_LE_INPUT LE_InputHandler::destroyInstance()

    /**
     * @brief clasificates the possible key states
     * */
    typedef enum keyState {
        pressed,
        released,
        iddle
    } keyState;

    typedef struct MouseInput {
        Uint32 windowId;
        keyState left;
        keyState middle;
        keyState right;
        Sint32 xrel;
        Sint32 yrel;
        Sint32 xabs;
        Sint32 yabs;
        float scrollx;
        float scrolly;
    } Mousestate;

    /**
     * @brief Handles all input-related events from SDL
     * */
    class LE_InputHandler
    {
        private:
            /**
             * @brief class constructor
             * */
            LE_InputHandler();

            /**
             * @brief class destructor
             * */
            ~LE_InputHandler() {}
            
            /**
             * @brief singleton instance
             * */
            static LE_InputHandler* the_instance;

            /**
             * @brief saves joystick information when they are initialized
             * */
            std::vector<SDL_Joystick*> joysticks;

            /**
             * @brief true when joysticks are initialized
             * */
            bool is_joysticks_initialized;

            /**
             * @brief saves keyboard key-states
             * */
            std::map<int, keyState> keys;

            /**
             * @brief saves mouse state
             * */
            MouseInput mouse;
        public:
            /**
             * @brief returns a ptr to the class instance
             * */
            static LE_InputHandler* Instance() {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_InputHandler();
                }
                return the_instance;
            }
            
            /**
             * @brief deallocates memory for LE_InputHandler
             * */
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            /**
             * @brief get reading access to the mouse state
             * */
            const MouseInput& getMouse () { return mouse; }

            void setMouseIddleLeft () { mouse.left = keyState::iddle; }
            void setMouseIddleRight () { mouse.right = keyState::iddle; }
            void setMouseIddleMiddle () { mouse.middle = keyState::iddle; }

            /**
             * @brief initializes connected joysticks
             * */
            void initJoysticks();
            bool isJoysticksInitialized() { return is_joysticks_initialized; }

            /**
             * @brief returns key state
             * */
            keyState getKeyState( int sym ) const { 
                auto it = keys.find(sym);
                if (it != keys.end()) {
                    return it->second;
                } else {
                    return keyState::iddle;
                }
            }

            /**
             * @brief Sets a key as iddle
             * */
            void setKeyIddle( int sym ) { 
                keys[sym] = keyState::iddle;
            }

            /**
             * @brief Poll SDL events
             * */
            void update();
            void clean();
    };

#endif
