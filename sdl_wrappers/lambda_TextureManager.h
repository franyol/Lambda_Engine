#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <map>
#include <string>

#ifndef _LAMBDA_ENGINE_TEXTURE_MANAGER_H_
#define _LAMBDA_ENGINE_TEXTURE_MANAGER_H_

    /*
     * Macro for retrieving the texture manager instance
     * */
    #define LE_TEXTURE LE_TextureManager::Instance()
    #define QUIT_LE_TEXTURE LE_TextureManager::destroyInstance()

    /*
     * Saves the tiles position for a texture id, useful to
     * generate a tilemap
     * */
    class LE_Tile
    {
        friend class LE_Texture;
        friend class LE_TextureManager;

        private:
            /*
             * Texture Id since a renderer may Have several SDL textures
             * */
            std::string textureId;
            /* x position in the tile map */
            int x;
            /* y position in the tile map */
            int y;
            /* height of the tile */
            int h;
            /* width of the tile */
            int w;

        public:
            LE_Tile ( std::string m_textureId, int m_x, int m_y, int m_h, int m_w ):
                      textureId(m_textureId), x(m_x), y(m_y), h(m_h), w(m_w) {}
            ~LE_Tile () {}

            /* 
             * Get tile information all at once
             * */
            void query ( std::string* m_textureId = nullptr,
                         int* m_x = nullptr, int* m_y = nullptr, 
                         int* m_h = nullptr, int* m_w = nullptr );
    };

    /*
     * In SDL2, textures, renderers and windows are bonded toguether
     * Here this bond will be explicit, grouping them all in a single class
     * */
    class LE_Texture
    {
        // Now the texture manager can access LE_Texture members directly
        friend class LE_TextureManager;

        private:
            /*
             * Window associated with the current texture
             * */
            SDL_Window*                         sdl_window;

            /*
             * Renderer associated with the current texture
             * */
            SDL_Renderer*                       sdl_renderer;

            /*
             * Loaded textures accessible by an string Id
             * */
            std::map<std::string, SDL_Texture*> sdl_textures;

            /*
             * Tile information of the loaded textures accessible
             * by string Id
             * */
            std::map<std::string, LE_Tile*>    tileSet;

        public:
            LE_Texture ( SDL_Window* win, SDL_Renderer* ren ) { 
                sdl_window = win; 
                sdl_renderer = ren;
            }
            ~LE_Texture () { clean(); }

            /*
             * Getter functions
             * */
            SDL_Window* getWindow () { return sdl_window; }
            SDL_Renderer* getRenderer () { return sdl_renderer; }

            /*
             * manage texture map
             * */
            void addTexture ( std::string texture_id, SDL_Texture* new_texture ) {
                sdl_textures[texture_id] = new_texture;
            }
            void popTexture ( std::string texture_id ) {
                auto it = sdl_textures.find(texture_id);
                if (it != sdl_textures.end()) {
                    SDL_DestroyTexture ( it->second );
                    sdl_textures.erase(it);
                }
            }
            SDL_Texture* getTexture ( std::string texture_id ) {
                auto it = sdl_textures.find(texture_id);
                if (it != sdl_textures.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /*
             * manage tile map
             * */
            void addTile ( std::string tile_id, LE_Tile* new_tile ) {
                tileSet[tile_id] = new_tile;
            }
            void popTile ( std::string tile_id ) {
                auto it = tileSet.find(tile_id);
                if (it != tileSet.end()) {
                    delete it->second;
                    tileSet.erase(it);
                }
            }
            LE_Tile* getTile ( std::string tile_id ) {
                auto it = tileSet.find(tile_id);
                if (it != tileSet.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /*
             * Free all memory from sdl_textures and tileSet
             * */
            void clean();

    };

    /*
     * The texture manager definitions, in charge of managing SDL initialization,
     * windows, renderers and textures.
     * */
    class LE_TextureManager
    {
        private:
            /* 
             * Saves wether SDL has been initialized by the texture manager 
             * */
            bool sdl_initialized;

            /* 
             * Saves wether SDL Image has been initialized by the texture manager 
             * */
            bool sdl_image_initialized;

            /* 
             * Saves wether SDL TTF has been initialized by the texture manager 
             * */
            bool sdl_ttf_initialized;

            /*
             * The numeric ID matches the window ID generated by SDL, useful when
             * handling events in a multiple-window environment.
             * */
            std::map<Uint32, LE_Texture*> windows;

            /*
             * The texture manager is designed to be a singleton, that's Why:
             * A static member saving the object pointer is required
             * */
            static LE_TextureManager* tm_instance;
            /*
             * And the class constructor is protected
             * ( Further class initialization is managed in
             *   init() method )
             * */
            LE_TextureManager () { 
                sdl_initialized = sdl_image_initialized = sdl_ttf_initialized = false;
                init(); 
            }

        public:
            /* 
             * In case that the singleton is destructed, it will
             * perform the pertinent cleanning including SDL
             * Quit functions at clean() method
             * */
            ~LE_TextureManager () { clean(); }

            /* 
             * Try to initialize SDL Libraries
             * */
            void init ();

            /* 
             * Deinitialize SDL Libraries if they are running
             * */
            void clean ();

            /*
             * Generate a new window, returns the window Id that
             * should be used to refference that window onwards
             *
             * This also creates a renderer for that window
             *
             * If the returned window ID is 0, then there was an error
             * during the window creation.
             * */
            Uint32 addWindow ( 
                   const char* title, 
                   int w, 
                   int h, 
                   bool full_screen = false, 
                   bool input_focus = false,
                   bool hidden = false,
                   bool borderless = false,
                   bool resizable = false );

            /*
             * Destroy a window by it's ID
             * */
            void popWindow ( Uint32 windowId ) {
                auto it = windows.find(windowId);
                if (it != windows.end()) {
                    delete it->second;
                    windows.erase(it);
                }
            }

            void getWindowSize ( Uint32 windowId, int* height, int* width ) {
                auto it = windows.find(windowId);
                if (it != windows.end()) {
                    SDL_GetWindowSize ( it->second->getWindow(), width, height );
                } 
            }

            /*
             * Load an sdl texture from a png file
             * */
            void loadTexture ( Uint32 windowId, 
                    std::string filePath, 
                    std::string textureId );

            void addTexture ( Uint32 windowId, std::string textureId, SDL_Texture* nT ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->addTexture( textureId, nT );
                }
            }

            void popTexture ( Uint32 windowId, std::string textureId ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->popTexture( textureId );
                }
            }

            /*
             * Adds a tile to the tile map
             * if h or w are entered as 0, the tile will
             * get the texture dimentions.
             * */
            void addTile ( Uint32 windowId,
                    std::string textureId,
                    std::string tileId,
                    int x = 0, int y = 0, int h = 0, int w = 0 );

            void popTile ( Uint32 windowId, std::string tileId ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->popTile( tileId );
                }
            }

            /* 
             * Getter functions to check initialization success
             * */
            bool SDLWasInit () { return sdl_initialized; }
            bool ImageWasInit () { return sdl_image_initialized; }
            bool TTFWasInit () { return sdl_ttf_initialized; }
            bool EverythingWasInit() { 
                return ( sdl_initialized && sdl_image_initialized && sdl_ttf_initialized ); 
            }

            /*
             * Singleton instance retriever
             * */
            static LE_TextureManager* Instance () {
                if ( tm_instance == nullptr ) {
                    tm_instance = new LE_TextureManager ();
                }
                return tm_instance;
            }
            static void destroyInstance () {
                if ( tm_instance != nullptr ) {
                    delete tm_instance;
                    tm_instance = nullptr;
                }
            }

            /*
             * Background filler
             * */
            bool fillBackground ( Uint32 windowId, Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    LE_Texture* leText = it->second;
                    SDL_SetRenderDrawColor ( leText->getRenderer(), r, g, b, a );
                    SDL_RenderClear ( leText->getRenderer() );
                    return true;
                }
                // Window for this id does not exist
                return false;
            }

            /*
             * Show all elements drawn onto the window
             * */
            bool present ( Uint32 windowId ) {
                auto it = windows.find(windowId);
                if ( it != windows.end() ) {
                    SDL_RenderPresent ( it->second->getRenderer() );
                    return true;
                }
                return false;
            }

            /* 
             * Draw a tile into a window.
             * The tile contains sdl rendered texture information
             * so no more info is needed.
             * */
            bool draw ( Uint32 windowId, std::string tileId,
                        int x, int y, double h = 1, double w = 1, 
                        bool scale = true, bool flipv = false,
                        bool fliph = false, const double angle = 0 );

            /*
             * Change target to a new texture, all draw functions will
             * be blended into this texture instead of the window until
             * restoreRenderTarget() is called
             * */
            void createTargetTexture ( Uint32 windowId, std::string textureId, 
                                       int h, int w ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) {
                    return;
                }

                SDL_Texture* targetTexture = SDL_CreateTexture ( 
                        it->second->getRenderer(), 
                        SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET,
                        w, h );
                addTexture ( windowId, textureId, targetTexture );

                SDL_SetRenderTarget ( it->second->getRenderer(), targetTexture );
            }

            void setRenderTarget ( Uint32 windowId, std::string textureId ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) return;
                
                SDL_SetRenderTarget ( it->second->getRenderer(), 
                        it->second->getTexture( textureId ) );
            }

            /*
             * Restore all the draw functions to draw to the window again
             * if crateTargetTexture() was called before
             * */
            void restoreRenderTarget ( Uint32 windowId ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) return;

                SDL_SetRenderTarget ( it->second->getRenderer(), NULL );
            }

            void getTileSize ( Uint32 windowId, std::string tileId, int* h, int* w ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) return;
                auto it2 = it->second->tileSet.find ( tileId );
                if ( it2 == it->second->tileSet.end() ) return;
                if ( h != nullptr ) *h = it2->second->h;
                if ( w != nullptr ) *w = it2->second->w;
            }
    };

#endif
