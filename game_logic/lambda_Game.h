#include "sdl_wrappers/lambda_TextureManager.h"
#include <vector>

#ifndef _LAMBDA_ENGINE_GAME_H_
#define _LAMBDA_ENGINE_GAME_H_

    #define LE_GAME LE_Game::Instance()
    #define QUIT_LE_GAME LE_Game::destroyInstance()

    class LE_Game
    {
        private:
            bool running;
            std::vector<Uint32> windows;

            bool framerateFixed;
            int framerate;

            double deltaTime;

            // Singleton
            static LE_Game* the_instance;
            LE_Game () {
                running = false;
                framerateFixed = false;
                framerate = 30;
            }

        public:
            ~LE_Game () { clean(); }

            // Singleton
            static LE_Game* Instance() {
                if ( the_instance == nullptr ) {
                    the_instance = new LE_Game();
                } 
                return the_instance;
            }

            static void destroyInstance() {
                if (the_instance != nullptr) {
                    delete the_instance;
                    the_instance = nullptr;
                }
            }

            // Time elapsed since last frame in milliseconds
            double getDeltaTime () { return deltaTime; }

            void fixFramerate ( int fps ) {
                framerate = fps;
                framerateFixed = true;
            }

            void unfixFramerate () { framerateFixed = false; }

            /*
             * Creates a new window and returns 
             * */
            Uint32 createWindow ( const char* title, int w, int h,
                   bool full_screen = false, bool input_focus = false,
                   bool hidden = false, bool borderless = false, 
                   bool resizable = false ) {
                Uint32 windowId = LE_TEXTURE->addWindow( title, w, h, 
                            full_screen, input_focus,
                           hidden, borderless, resizable );
                windows.push_back ( windowId );
                return windowId;
            }

            Uint32 getWindow ( int idx ) {
                return windows.at(idx);
            }

            bool isRunning ( void ) { return running; }
            void setRunning ( bool state ) { running = state; }

            void handleEvents ();
            void update ();
            void render ();
            void clean  ();
            void exit   ();

            void mainLoop ();
    };

#endif
