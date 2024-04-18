#include <string>
#include <map>
#include "lambda_TextureManager.h"

#ifndef _LAMBDA_ENGINE_GAMEOBJ_H_
#define _LAMBDA_ENGINE_GAMEOBJ_H_

    /**
     * @brief groups a tile and a window Id
     * */
    typedef struct LE_Frame {
        std::string tileId;
        Uint32 windowId;
    } LE_Frame;

    /**
     * @brief Base class for creating your game objects
     *
     * A game object is basically anything that needs to be
     * updated and drawn into screen each game frame
     * */
    class LE_GameObject
    {
        friend class LE_GameState;

        protected:
            // protected types sould be accessible from inherited classes
            // so no getter functions are needed
            double x;
            double y;
            double h;
            double w;
            double angle;
            bool scale;
            bool flipv;
            bool fliph;

            /**
             * @brief when true, object will be deleted in the next frame update
             * */
            bool destroy_me;

            /**
             * @brief stores tiles as frames related to a game object
             * */
            std::map<std::string, LE_Frame> frames;

            /** 
             * @brief Intended for single sprite animation
             * 
             * to animate more than one sprites you may need to
             * define a new map using this member as the id, similar
             * to frames memeber implementation
             * */
            std::string currentFrame;
        public:
            
            /**
             * @brief class constructor
             * */
            LE_GameObject () {}
            virtual ~LE_GameObject () {}
            
            /**
             * @brief add your class initialization here
             *
             * the setup method is called when you add the object to a game state,
             * use it to initialize object variables.
             *
             * use LE_GameObject::setup(); inside your own function to
             * inherit these default definitions.
             *
             * Make sure to define destroy_me to false here, or your obect
             * will be destroyed immediately before even being able to be 
             * drawn into screen
             * */
            virtual void setup () {
                // Initialize these members when redefining this function
                // or just run LE_GameObject::setup(); and then add your own
                // setup
                x = y = 0;
                h = w = 1;
                angle = 0;
                scale = true;
                flipv = fliph = false;
                destroy_me = false;
            }

            /**
             * @brief object update function
             *
             * Use this method to update game object's members
             * acording to user input or your game necessities
             * */
            virtual void update () {}

            /**
             * @brief render game object into the window
             *
             * By default, renders the currentFrame into the window
             *
             * Overload it if you want to override this functionality
             * */
            virtual void render () {
                auto it = frames.find( currentFrame );
                if ( it == frames.end() ) return;

                LE_TEXTURE->draw ( it->second.windowId, it->second.tileId,
                       x, y, h, w, scale, flipv, fliph, angle );
            }
    };

#endif
