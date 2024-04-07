#include <SDL2/SDL.h>
#include <vector>
#include <map>

#ifndef _LAMBDA_INPUT_HANDLER_H_
#define _LAMBDA_INPUT_HANDLER_H_

    #define LE_INPUT LE_InputHandler::Instance()
    #define QUIT_LE_INPUT LE_InputHandler::destroyInstance()

    typedef enum keyState {
        pressed,
        released,
        iddle
    } keyState;

    class LE_InputHandler
    {
        private:
            LE_InputHandler();
            ~LE_InputHandler() {}
            
            // Singleton
            static LE_InputHandler* the_instance;

            std::vector<SDL_Joystick*> joysticks;
            bool is_joysticks_initialized;

            std::map<int, keyState> keys;
        public:
            static LE_InputHandler* Instance() {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_InputHandler();
                }
                return the_instance;
            }
            
            static void destroyInstance () {
                if ( the_instance != nullptr ) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            void initJoysticks();
            bool joysticksInitialized() { return is_joysticks_initialized; }

            keyState getKeyState( int sym ) const { 
                auto it = keys.find(sym);
                if (it != keys.end()) {
                    return it->second;
                } else {
                    return keyState::iddle;
                }
            }
            void setKeyState( int sym, keyState state ) { 
                keys[sym] = state;
            }

            void update();
            void clean();
    };

#endif
