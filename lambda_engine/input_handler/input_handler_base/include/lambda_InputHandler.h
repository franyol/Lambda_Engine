#include <SDL2/SDL.h>
#include <array>
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
    enum class keyState {
        pressed,
        released,
        idle
    };

    /**
     * @brief stores mouse input information
     * */
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
        bool was_moved;
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
            std::array<keyState, SDL_NUM_SCANCODES> keys;

            std::vector<SDL_Scancode> releasedKeys;

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
            MouseInput* getMouse () { return &mouse; }

            void setMouseIdleLeft () { mouse.left = keyState::idle; }
            void setMouseIdleRight () { mouse.right = keyState::idle; }
            void setMouseIdleMiddle () { mouse.middle = keyState::idle; }

            /**
             * @brief sets every keystate to iddle after one loop in release
             * */
            void setReleasedToIddle() {
                setMouseIdleMiddle();
                setMouseIdleRight();
                setMouseIdleLeft();

                for (auto& scancode: releasedKeys) {
                    keys[scancode] = keyState::idle;
                }
                releasedKeys.clear();
            }

            /**
             * @brief initializes connected joysticks
             * */
            void initJoysticks();
            bool isJoysticksInitialized() { return is_joysticks_initialized; }

            /**
             * @brief returns key state
             * */
            keyState getKeyState(int scancode) const {

                if (scancode < 0 || scancode >= SDL_NUM_SCANCODES) {
                    return keyState::idle;
                }

                return keys[scancode];
            }

            /**
             * @brief Sets a key as iddle
             * */
            void setKeyIdle(SDL_Scancode scancode) {

                if (scancode >= SDL_NUM_SCANCODES) {
                    return;
                }

                keys[scancode] = keyState::idle;
            }

            /**
             * @brief Poll SDL events
             * */
            void update();
            void clean();
    };

#endif
