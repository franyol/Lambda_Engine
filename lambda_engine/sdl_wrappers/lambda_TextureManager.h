#ifndef _LAMBDA_ENGINE_TEXTURE_MANAGER_H_
#define _LAMBDA_ENGINE_TEXTURE_MANAGER_H_

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <vector>
    #include <map>
    #include <string>
    #include <iostream>

    /**
     * @brief Shortcut to calling the texture manager instance
     *
     * If memory is not a concern, one can save the texture manager
     * instance in a LE_TextureManager* variable to enhance execution
     * time.
     * */
    #define LE_TEXTURE LE_TextureManager::Instance()

    /**
     * @brief Shortcut to calling the texture manager destructor
     *
     * Called automatically when \ref LE_Quit "LE_Quit()" is called.
     * */
    #define QUIT_LE_TEXTURE LE_TextureManager::destroyInstance()

    /**
     * @brief enumerate SDL available blend modes
     *
     * For blend mode details: https://wiki.libsdl.org/SDL2/SDL_BlendMode
     * | LE_BlendMode | SDL_BlendMode equivalent |
     * |--------------|--------------------------|
     * | none         | SDL_BLENDMODE_NONE       |
     * | blend        | SDL_BLENDMODE_BLEND      |
     * | add          | SDL_BLENDMODE_ADD        |
     * | mod          | SDL_BLENDMODE_MOD        |
     * | mul          | SDL_BLENDMODE_MUL        |
     * */
    typedef enum LE_BlendMode {
        none =  SDL_BLENDMODE_NONE,
        blend = SDL_BLENDMODE_BLEND,
        add =   SDL_BLENDMODE_ADD,
        mod =   SDL_BLENDMODE_MOD,
        mul =   SDL_BLENDMODE_MUL
    } LE_BlendMode;

    /**
     * @brief Stores tile location info
     *
     * A tile is a rectangular section of a SDL_Texture, LE_Tile objects
     * stores necessary information to draw a tile into the screen.
     * */
    class LE_Tile
    {
        friend class LE_Window;
        friend class LE_TextureManager;

        private:
            /**
             * @brief Id of the texture from which the tile will be extracted.
             *
             * When calling \ref LE_TextureManger::draw "LE_TextureManager::draw()"
             * you can  use the same `LE_Tile` object to draw a tile in two different
             * windows if they share an onject with the same textureId.
             *
             * For that reason, windowId is not an existing member of LE_Tile class.
             * 
             * @see LE_TextureManager::draw
             * Example: Using the same LE_Tile object for two windows:
             *
             * \code
             * std::string textureId = "tilemap";
             * LE_Tile* sharedTile = new LE_Tile ( textureId, 0, 0, 20, 20 );
             *
             * LE_TEXTURE->addTile ( window1, "tile1", sharedTile );
             * LE_TEXTURE->addTile ( window2, "tile1", sharedTile );
             * // Now both window1 and window2 can draw the shared tile
             * // as long as they both have a texture with id: "tilemap"
             * \endcode
             * */
            std::string textureId;

            /** @brief x position in pixels */
            int x;
            /** @brief y position in pixels */
            int y;
            /** @brief height in pixels */
            int h;
            /** @brief width in pixels */
            int w;

        public:
            /**
             * @brief Class constructor
             *
             * @param m_textureId Texture id
             * @param m_x x pos in pixels
             * @param m_y y pos in pixels
             * @param m_h height in pixels
             * @param m_w width in pixels
             * */
            LE_Tile ( std::string m_textureId, int m_x, int m_y, int m_h, int m_w ):
                      textureId(m_textureId), x(m_x), y(m_y), h(m_h), w(m_w) {}
            /**
             * @brief Class destructor
             * */
            ~LE_Tile () {}

            /** 
             * @brief Get class members
             *
             * It's not expected to get or modify any of LE_Tile members
             * since tilemaps are static assets, use this function in case
             * you need to query the tile data.
             *
             * @param m_textureId ptr where to save Texture id
             * @param m_x ptr where to save x cords
             * @param m_y ptr where to save y cords
             * @param m_h ptr where to save height
             * @param m_w ptr where to save width
             * */
            void query ( std::string* m_textureId = nullptr,
                         int* m_x = nullptr, int* m_y = nullptr, 
                         int* m_h = nullptr, int* m_w = nullptr );
    };

    /**
     * @brief Class for grouping window-dependent elements
     *
     * In SDL2, textures, renderers and windows are bonded together
     * Here this bond explicitly groups them all in a single class
     *
     * This way, a LE_Window, has an SDL_Window* member which gives
     * it access to SDL functionality; a SDL_Renerer* member which can
     * only work for the window it was created; a SDL_Texture* mapping
     * to sort by ID all the textures created for that renderer (which
     * can'y neither be used in a different one); and finally a mapping
     * of LE_Tile* objects, which, as we stated in LE_Tile class, 
     * can be used in several windows (if they both share textureId's and
     * probably also loaded the same textures for their own renderer)
     * */
    class LE_Window
    {
        friend class LE_TextureManager;

        private:
            /**
             * @brief SDL window instance
             * */
            SDL_Window*                         sdl_window;

            /**
             * @brief SDL Renderer instance
             * */
            SDL_Renderer*                       sdl_renderer;

            /**
             * @brief Loaded textures accessible by Id
             * */
            std::map<std::string, SDL_Texture*> sdl_textures;

            /**
             * @brief LE_Tile map ordered by id
             * */
            std::map<std::string, LE_Tile*>     tileSet;

        public:
            /**
             * @brief Class constructor
             *
             * @param win SDL_Window
             * @param ren SDL_Renderer generated from that window
             * */
            LE_Window ( SDL_Window* win, SDL_Renderer* ren ) { 
                sdl_window = win; 
                sdl_renderer = ren;
            }
            /**
             * @brief Class destructor
             *
             * calls LE_Window::clean()
             * also destroys window and renderer
             *
             * @see LE_Window::clean
             * */
            ~LE_Window () { 
                clean(); 
                SDL_DestroyRenderer( sdl_renderer );
                SDL_DestroyWindow( sdl_window );
            }

            /**
             * @brief sdl_window getter function
             * */
            SDL_Window* getWindow () { return sdl_window; }

            /**
             * @brief sdl_renderer getter function
             * */
            SDL_Renderer* getRenderer () { return sdl_renderer; }

            /**
             * @brief add a texture to LE_Window::sdl_textures
             *
             * @param textureId Id to refference that texture
             * @param newTexture SDL_Texture to add
             * */
            void addTexture ( std::string textureId, SDL_Texture* newTexture ) {
                auto it = sdl_textures.find( textureId );
                if ( it != sdl_textures.end() ) {
                    std::cerr << "The texture ID: " << textureId << " is already in use"
                        << ". Must deallocate previous texture before reloading it"
                        << std::endl;
                    return;
                }
                sdl_textures[textureId] = newTexture;
            }

            /**
             * @brief pop a texture from LE_Window::sdl_textures
             *
             * @param textureId texture to pop
             * */
            void popTexture ( std::string textureId ) {
                auto it = sdl_textures.find(textureId);
                if (it != sdl_textures.end()) {
                    SDL_DestroyTexture ( it->second );
                    sdl_textures.erase(it);
                }
            }

            /**
             * @brief get textures by id
             *
             * Returns a nullptr if the texture doesn't exist
             *
             * @param textureId
             * @return SDL_Texture* instance
             * */
            SDL_Texture* getTexture ( std::string textureId ) {
                auto it = sdl_textures.find(textureId);
                if (it != sdl_textures.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /**
             * @brief add a tile to LE_Window::tileSet
             *
             * @param textureId Id to refference that texture
             * @param newTexture SDL_Texture to add
             * */
            void addTile ( std::string tile_id, LE_Tile* new_tile ) {
                tileSet[tile_id] = new_tile;
            }

            /**
             * @brief remove a tile from LE_Window::tileSet by id
             *
             * Calling LE_Window::popTile also frees memory
             * allocated for the LE_Tile object
             *
             * @param tileId
             * */
            void popTile ( std::string tileId ) {
                auto it = tileSet.find(tileId);
                if (it != tileSet.end()) {
                    delete it->second;
                    tileSet.erase(it);
                }
            }

            /**
             * @brief get Tile by id
             * 
             * Returns a nullptr if there is no tile associated with
             * the tileId provided.
             *
             * @param tileId
             * @return LE_Tile
             * */
            LE_Tile* getTile ( std::string tileId ) {
                auto it = tileSet.find(tileId);
                if (it != tileSet.end()) {
                    return it->second;
                }
                return nullptr;
            }

            /**
             * @brief Free memory allocated for sdl_textures and tileSet
             *
             * Clears sdl_textures and tileSet maps
             * */
            void clean();

    };

    /**
     * @brief Manages Window, Texture and Tile creation in a single project
     *
     * This class is a singleton, the LE_TextureManager instance is initialized
     * when LE_Init is called. To get access  to the instance use 
     * LE_TextureManager::Instance() static method, or the macro LE_TEXTURE which
     * calls the same function.
     *
     * @code
     * // Save the instance to a variable to avoid calling LE_TextureManager::Instance
     * // Every time, specially for loops.
     * LE_TextureManager* tM = LE_TEXTURE;
     *
     * // Or use it directly and save some memory at cost of performance
     * // Preffer this option if you just want to load some textures
     * LE_TEXTURE->init();
     * @endcode
     * */
    class LE_TextureManager
    {
        private:

            /**
             * @brief Saves wether SDL has been initialized by the texture manager 
             * */
            bool sdl_initialized;

            /** 
             * @brief Saves wether SDL Image has been initialized by the texture manager 
             * */
            bool sdl_image_initialized;

            /** 
             * @brief Saves wether SDL TTF has been initialized by the texture manager 
             * */
            bool sdl_ttf_initialized;

            /**
             * @brief Stores project's active windows
             *
             * The numeric ID matches the window ID generated by SDL, useful when
             * handling events in a multiple-window environment.
             *
             * Save the window ID's in Uint32 type variables:
             * @code
             * Uint32 mainWindow = LE_Texture->createWindow( "My Window", 480, 640 );
             * Uint32 secondWindow = LE_Texture->createWindow( "Second", 480, 640 );
             * @endcode
             * */
            std::map<Uint32, LE_Window*> windows;

            /**
             * @brief saves the singleton's instance
             * */
            static LE_TextureManager* tm_instance;

            /**
             * @brief Class constructor
             *
             * Initializes SDL, SDL Image and SDL TTF
             * */
            LE_TextureManager () { 
                sdl_initialized = sdl_image_initialized = sdl_ttf_initialized = false;
                init(); 
            }

        public:
            /**
             * @brief Class destructor
             *
             * In case that the singleton is destructed, it will
             * perform the pertinent cleanning using the clean() method.
             * */
            ~LE_TextureManager () { clean(); }

            /** 
             * @brief Initializes SDL and it's extensions
             * */
            void init ();

            /**
             * @brief Deinitialize SDL Libraries
             * */
            void clean ();

            /**
             * @brief creates a new Window and Renderer
             *
             * This also creates a renderer for that window
             *
             * If the returned window ID is 0, then there was an error
             * during the window creation.
             *
             * @param title Window title
             * @param h Window Height
             * @param w Window width
             * @param full_screen Set window to full screen
             * @param input_focus Set Input focus on this window
             * @param hidden Hide this window when created
             * @param resizable Window is resizable
             * */
            Uint32 createWindow ( 
                   const char* title, 
                   int h, 
                   int w, 
                   bool full_screen = false, 
                   bool input_focus = false,
                   bool hidden = false,
                   bool borderless = false,
                   bool resizable = false );

            /**
             * @brief Destroy a window by it's ID
             *
             * @param windowId
             * */
            void popWindow ( Uint32 windowId ) {
                auto it = windows.find(windowId);
                if (it != windows.end()) {
                    delete it->second;
                    windows.erase(it);
                }
            }

            /**
             * @brief Get the size of a window
             *
             * @param windowId
             * @param height int pointer where to save the window height
             * @param width int pointer where to save the window widht
             * */
            void getWindowSize ( Uint32 windowId, int* height, int* width ) {
                auto it = windows.find(windowId);
                if (it != windows.end()) {
                    SDL_GetWindowSize ( it->second->getWindow(), width, height );
                } 
            }

            /**
             * @brief Get window renderer
             *
             * @param windowId
             * @return SDL_Renderer*
             * */
            SDL_Renderer* getRenderer ( Uint32 windowId ) {
                auto it = windows.find(windowId);
                if (it != windows.end()) {
                    return it->second->getRenderer();
                } 
                return nullptr;
            }

            /**
             * @brief Load an sdl texture from a png file
             *
             * @param windowId
             * @param filePath PNG file path
             * @param textureId the generated texture will be refferenced by it
             * */
            void loadTexture ( Uint32 windowId, 
                    std::string filePath, 
                    std::string textureId );

            /**
             * @brief Add a texture from an existing SDL_Texture  object
             *
             * @param windowId
             * @param textureId the texture will be accessible by this Id
             * @param nT SDL_Texture* object
             * */
            void addTexture ( Uint32 windowId, std::string textureId, SDL_Texture* nT ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->addTexture( textureId, nT );
                }
            }

            /**
             * @brief Remove a texture by it's ID
             *
             * @param windowId
             * @param textureId
             * */
            void popTexture ( Uint32 windowId, std::string textureId ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->popTexture( textureId );
                }
            }

            /**
             * @brief add a Tile from existing LE_Tile object
             *
             * @param windowId
             * @param tileId Id for the new tile
             * @param newTile LE_Tile object to add
             * */
            void addTile ( Uint32 windowId, std::string tileId, LE_Tile* newTile ) {
                auto it = windows.find( windowId );
                if ( it == windows.end() ) {
                    std::cerr << "Error adding tile: window id " << windowId <<
                        " doesn't exist" << std::endl;
                    return;
                }
                it->second->addTile ( tileId, newTile );
            }

            /**
             * @brief Adds a tile to the tile map
             *
             * if h or w are entered as 0, the tile will
             * match the texture size
             *
             * @param windowId
             * @param textureId
             * @param tileId
             * @param x tile x coord
             * @param y tile y coord
             * @param h tile height
             * @param w tile width
             * */
            void createTile ( Uint32 windowId,
                    std::string textureId,
                    std::string tileId,
                    int x = 0, int y = 0, int h = 0, int w = 0 );

            /**
             * @brief Delete tile by Id
             *
             * @param windowId
             * @param tileId
             * */
            void popTile ( Uint32 windowId, std::string tileId ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    it->second->popTile( tileId );
                }
            }

            /**
             * @brief Chekc if sdl is initialized
             *
             * @return true sdl is initialized
             * */
            bool SDLWasInit () { return sdl_initialized; }

            /**
             * @brief Chekc if sdl_image is initialized
             *
             * @return true sdl_image is initialized
             * */
            bool ImageWasInit () { return sdl_image_initialized; }

            /**
             * @brief Chekc if sdl_ttf is initialized
             *
             * @return true sdl_ttf is initialized
             * */
            bool TTFWasInit () { return sdl_ttf_initialized; }

            /**
             * @brief Check if all sdl subsystems were initialized
             *
             * @return true if all subsystems were initialized
             * */
            bool EverythingWasInit() { 
                return ( sdl_initialized && sdl_image_initialized && sdl_ttf_initialized ); 
            }

            /**
             * @brief Returns the LE_TextureManager instance
             * */
            static LE_TextureManager* Instance () {
                if ( tm_instance == nullptr ) {
                    tm_instance = new LE_TextureManager ();
                }
                return tm_instance;
            }

            /**
             * @brief Destroy LE_TextureManager instance
             *
             * called inside LE_Quit()
             * */
            static void destroyInstance () {
                if ( tm_instance != nullptr ) {
                    delete tm_instance;
                    tm_instance = nullptr;
                }
            }

            /**
             * @brief Fill background with a plain rgb color
             * */
            bool fillBackground ( Uint32 windowId, Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
                auto it = windows.find( windowId );
                if ( it != windows.end() ) {
                    LE_Window* leText = it->second;
                    SDL_SetRenderDrawColor ( leText->getRenderer(), r, g, b, a );
                    SDL_RenderClear ( leText->getRenderer() );
                    return true;
                }
                // Window for this id does not exist
                return false;
            }

            /**
             * @brief Renders all elements that have been drawn onto the window
             *
             * @param windowId
             * */
            bool present ( Uint32 windowId ) {
                auto it = windows.find(windowId);
                if ( it != windows.end() ) {
                    SDL_RenderPresent ( it->second->getRenderer() );
                    return true;
                }
                return false;
            }

            /**
             * @brief draw a tile
             *
             * The tile is only drawn into the window corresponding to
             * the windowId inserted and also, the textureId referenced
             * into that tile must exist in that window sdl_textures map
             *
             * Note: in order to the tiles drawn to be shown in a window,
             * call LE_TextureManager::present(windowId)
             *
             * @param windowId
             * @param tileId
             * @param x coord to draw the tile into the window
             * @param y coord to draw the tile into the window
             * @param h if scale is set to true, works a a multipier for the actual tile height, it works as an absolute height modifier.
             * @param w if scale is set to true, works a a multipier for the actual tile width, it works as an absolute width modifier.
             * @param scale set to false to set h and w as absolute size modifier
             * @param flipv flip the image vertically
             * @param fliph flip the image horizontally
             * @param angle
             * @return true if the draw was completed without error
             * */
            bool draw ( Uint32 windowId, std::string tileId,
                        int x, int y, double h = 1, double w = 1, 
                        bool scale = true, bool flipv = false,
                        bool fliph = false, const double angle = 0 );

            /**
             * @brief change draw target to an empty texture
             *
             * Change target to a new texture, all draw functions will
             * be blended into this texture instead of the window until
             * LE_TextureManager::restoreRenderTarget() is called
             *
             * @param windowId
             * @param textureId texture to be set as target
             * 
             * Example: Blend two tiles together
             * @code
             * #include <lambda.h>
             *
             * int main ( int argc, char* argv[] ) {
             *     LE_Init();
             *     // Create a window to show the texture
             *     Uint32 mainWindow = LE_TEXTURE->createWindow( "Blend", 640, 480 );
             *
             *     // Load two images into the texture manager
             *     LE_TEXTURE->loadTexture( mainWindow, "myimage1.png", "im1" );
             *     LE_TEXTURE->loadTexture( mainWindow, "myimage2.png", "im2" );
             *     // Create tiles for those textures
             *     LE_TEXTURE->createTile( mainWindow, "im1", "im1_tile" );
             *     LE_TEXTURE->createTile( mainWindow, "im2", "im2_tile" );
             *
             *     // Create a new texture and tile to blend the tiles:
             *     LE_TEXTURE->createTargetTexture( mainWindow, "im3", 640, 480 );
             *     LE_TEXTURE->createTile( mainWindow, "im3", "im3_tile" );
             *
             *     // Now when we draw, we draw into the texture instead of the window
             *     LE_TEXTURE->draw( mainWindow, "im1_tile", 0, 0, 640, 480, false );
             *     LE_TEXTURE->draw( mainWindow, "im2_tile", 0, 0, 320, 240, false );
             *
             *     // Restore the target to the main window
             *     LE_TEXTURE->restoreRenderTarget( mainWindow );
             *
             *     // display the blended texture into the window
             *     LE_TEXTURE->draw( mainWindow, "im3_tile", 0, 0 );
             *     LE_TEXTURE->present( mainWindow );
             *
             *     // Mantain the window during 5 seconds
             *     SDL_Delay ( 5000 );
             *     LE_Quit();
             *     return 0;
             * }
             * @endcode
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

            /**
             * @brief sets render target to a texture
             *
             * The texture must have been created using 
             * LE_TextureManager::createTargetTexture
             *
             * @param windowId
             * @param textureId
             * */
            void setRenderTarget ( Uint32 windowId, std::string textureId ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) return;
                
                SDL_SetRenderTarget ( it->second->getRenderer(), 
                        it->second->getTexture( textureId ) );
            }

            /**
             * @brief restore the render target to point draws into the window
             *
             * @param windowId
             * */
            void restoreRenderTarget ( Uint32 windowId ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) return;

                SDL_SetRenderTarget ( it->second->getRenderer(), NULL );
            }

            /**
             * @brief set SDL renderer blend mode
             *
             * @param blendMode LE_BlendMode
             * @param windowId window to apply the blendmode, if no other option is 
             * applied, the blend mode is applied to the renderer, use 
             * LE_BlendMode::none to restore it.
             * @param textureId optional; if set, the blendmode is only applied 
             * for this texture
             * */
            void setBlendMode ( LE_BlendMode blendMode, 
                    Uint32 windowId, 
                    std::string textureId = "" );

            /**
             * @brief get tile's height and width
             *
             * @param windowId
             * @param tileId
             * @param h pointer where the height data will be stored
             * @param w pointer where the width data will be stored
             * */
            void getTileSize ( Uint32 windowId, std::string tileId, int* h, int* w ) {
                auto it = windows.find ( windowId );
                if ( it == windows.end() ) {
                    std::cerr << "Could not get tile size for " << tileId
                        << " Window ID: " << windowId << " Doesn't exist." << std::endl;
                    return;
                }
                auto it2 = it->second->tileSet.find ( tileId );
                if ( it2 == it->second->tileSet.end() ) {
                    std::cerr << "Could not get tile size for " << tileId
                        << " Tile ID  Doesn't exist." << std::endl;
                    return;
                }
                if ( h != nullptr ) *h = it2->second->h;
                if ( w != nullptr ) *w = it2->second->w;
            }

            /**
             * @brief load Textures and tiles from xml file
             *
             * @param filePath path to xml file
             * @param windowId
             *
             * XML file format:
             * @code
             * <TILESETS>
             *   <texture id="myTexture" filepath="path/to/texture.png">
             *       <set tile="myTile1" x="0" y="0" h="24" w="20"/>
             *       <set tile="myTile2" x="20" y="0" h="24" w="20"/>
             *   </texture>
             *   <texture id="myTexture2" filepath="path/to/texture2.png">
             *       <set tile="myTile3" x="0" y="0" h="24" w="20"/>
             *       <set tile="myTile4" x="20" y="0" h="24" w="20"/>
             *   </texture>
             * </TILESETS>
             * @endcode
             * */
            void loadFromXmlFile ( std::string filePath, Uint32 windowId );
    };

#endif
