#ifndef _LAMBDA_ENGINE_GAME_H_
#define _LAMBDA_ENGINE_GAME_H_

    #include "lambda_TextureManager.h"
    #include <vector>

    /**
     * @brief Shortcut to calling the LE_Game instance
     * */
    #define LE_GAME LE_Game::Instance()

    /**
     * @brief Shortcut to calling the LE_Game destructor
     * */
    #define QUIT_LE_GAME LE_Game::destroyInstance()

    /**
     * @brief Holds the base game structure, defines the main loop functionality
     * */
    class LE_Game
    {
        private:
            /**
             * @brief it's true while the main loop must keep iterating
             * */
            bool running;

            /**
             * @brief stores all the id's of the windows generated for the game
             * */
            std::vector<Uint32> windows;

            /**
             * @brief If ture, the framerate is fixed to LE_Game::framerate
             * */
            bool framerateFixed;

            /**
             * @brief states the maximum rate the game can update when 
             * LE_Game::framerateFixed is set
             * */
            int framerate;

            /**
             * @brief stores how much time has passed since the last frame update
             * */
            double deltaTime;

            /**
             * @brief Stores the LE_Game instance
             * */
            static LE_Game* the_instance;

            /**
             * @brief class constructor
             * */
            LE_Game () {
                running = false;
                framerateFixed = false;
                framerate = 30;
            }

        public:
            /**
             * @brief class destructor
             *
             * Calls LE_Game::clean
             * */
            ~LE_Game () { clean(); }

            /**
             * @brief Get LE_Game::the_instance
             *
             * @return LE_Game* pointer to the instance
             * */
            static LE_Game* Instance() {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_Game();
                } 
                return the_instance;
            }

            /**
             * @brief Deallocates memory from LE_Game::the_instance
             * */
            static void destroyInstance() {
                if (the_instance != nullptr) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            /**
             * @brief get LE_Game::deltaTime
             * */
            double getDeltaTime () { return deltaTime; }

            /**
             * @brief Set the framerate to a static value
             *
             * @param fps framerate in Hz
             * */
            void fixFramerate ( int fps ) {
                framerate = fps;
                framerateFixed = true;
            }

            /**
             * @brief Unfix framerate (game will update as fast as it can get)
             * */
            void unfixFramerate () { framerateFixed = false; }

            /**
             * @brief Creates a new window
             *
             * @param title window title
             * @param w window width in pixels
             * @param h window height in pixels
             * @param full_screen Expand the window to fill the screen
             * @param input_focus Sets the focus to this window
             * @param borderless Eliminates the window borderw (and close button)
             * @param resizable User can resize the window
             * */
            Uint32 createWindow ( const char* title, int w, int h,
                   bool full_screen = false, bool input_focus = false,
                   bool hidden = false, bool borderless = false, 
                   bool resizable = false ) {
                Uint32 windowId = LE_TEXTURE->createWindow( title, w, h, 
                            full_screen, input_focus,
                           hidden, borderless, resizable );
                windows.push_back ( windowId );
                return windowId;
            }

            /**
             * @brief get window Id
             *
             * @param idx index of the windows of LE_Game stored by it's creation order
             * @return window id
             * */
            Uint32 getWindow ( int idx ) {
                return windows.at(idx);
            }

            /**
             * @brief returns if the game loop is still running
             * */
            bool isRunning ( void ) { return running; }

            /**
             * @brief Change the game run state
             *
             * @param state true to continue game loop, false to stop it
             * */
            void setRunning ( bool state ) { running = state; }

            /**
             * @brief Calls LE_InputHandler::update()
             * */
            void handleEvents ();

            /**
             * @brief Updates LE_StateMachine LE_State Objects
             * */
            void update ();

            /**
             * @brief Clears the background, render all active objects and presents them
             * to the window they each belong to
             * */
            void render ();

            /**
             * @brief deallocates class memory
             * */
            void clean  ();

            /**
             * @brief stop and cleans the game at any point
             * */
            void exit   ();

            /**
             * @brief Starts Game main loop
             * */
            void mainLoop ();
    };

#endif
