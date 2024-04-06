#include <string>
#include <map>
#include "sdl_wrappers/lambda_TextureManager.h"

#ifndef _LAMBDA_ENGINE_GAMEOBJ_H_
#define _LAMBDA_ENGINE_GAMEOBJ_H_

    typedef struct LE_Frame {
        std::string tileId;
        Uint32 windowId;
    } LE_Frame;

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

            bool destroy_me;

            // So a single game object can have frames in different windows
            std::map<std::string, LE_Frame> frames;

            // Intended for single sprite animation
            // to animate more than one sprites you may need to
            // define a new map using this member as the id, similar
            // to frames memeber implementation
            std::string currentFrame;
        public:
            
            LE_GameObject () {}
            virtual ~LE_GameObject () {}
            
            // Customizable methods
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
            virtual void update () {}
            virtual void render () {
                auto it = frames.find( currentFrame );
                if ( it == frames.end() ) return;

                LE_TEXTURE->draw ( it->second.windowId, it->second.tileId,
                       x, y, h, w, scale, flipv, fliph, angle );
            }
    };

#endif
