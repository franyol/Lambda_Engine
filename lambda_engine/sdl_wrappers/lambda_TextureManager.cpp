#include "lambda_TextureManager.h"
#include <iostream>

using namespace std;

LE_TextureManager* LE_TextureManager::tm_instance;

void LE_Tile::query ( std::string* m_texture, int* m_x, int* m_y, int* m_h, int* m_w ) {
    if ( m_texture != nullptr ) *m_texture = textureId;
    if ( m_x != nullptr ) *m_x = x;
    if ( m_y != nullptr ) *m_y = y;
    if ( m_h != nullptr ) *m_h = h;
    if ( m_w != nullptr ) *m_w = w;
}

void LE_Texture::clean() {
    for ( auto it = sdl_textures.begin(); it != sdl_textures.end(); it++ ) {
        SDL_DestroyTexture( it->second );
    }
    sdl_textures.clear();
    
    for ( auto it = tileSet.begin(); it != tileSet.end(); it++ ) {
        delete it->second;
    }
    tileSet.clear();
    SDL_DestroyWindow( sdl_window );

    // Exit SDL
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void LE_TextureManager::init() {

    if ( !sdl_initialized ) {
        if ( SDL_InitSubSystem ( SDL_INIT_EVERYTHING ) < 0 ) {
            cerr << "Error initializing SDL: " << SDL_GetError() << endl;
        } else {
            sdl_initialized = true;
        }
    }
    if ( sdl_initialized && !sdl_image_initialized ) {
        int imgFlags = IMG_INIT_PNG;
        if ( !(IMG_Init(imgFlags) & imgFlags) ) {
            cerr << "Error initializing SDL Image: " << IMG_GetError() << endl;
        } else {
            sdl_image_initialized = true;
        }
    }
    if ( sdl_initialized && !sdl_ttf_initialized ) {
        if ( TTF_Init() == -1 ) {
            cerr << "Error initializing SDL TTF: " << TTF_GetError() << endl;
        } else {
            sdl_ttf_initialized = true;
        }
    }
}

void LE_TextureManager::clean () {
    for ( auto it = windows.begin(); it != windows.end(); it++ ) {
        delete it->second;
    }
    windows.clear();
}

Uint32 LE_TextureManager::addWindow (
        const char* title,
        int w,
        int h,
        bool full_screen,
        bool input_focus,
        bool hidden,
        bool borderless,
        bool resizable
        ) {
    SDL_Window* newWindow;
    SDL_Renderer* newRenderer;

    Uint32 winFlags = 0;
    if ( full_screen ) winFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if ( input_focus ) winFlags |= SDL_WINDOW_INPUT_FOCUS;
    if ( hidden )      winFlags |= SDL_WINDOW_HIDDEN;
    if ( borderless )  winFlags |= SDL_WINDOW_BORDERLESS;
    if ( resizable )   winFlags |= SDL_WINDOW_RESIZABLE;

    newWindow = SDL_CreateWindow ( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   w, h, winFlags );
    if ( newWindow == NULL ) {
        cerr << "Error initializing window: " << SDL_GetError() << endl;
        return 0;
    }

    newRenderer = SDL_CreateRenderer ( newWindow, -1, SDL_RENDERER_ACCELERATED );
    if ( newRenderer == NULL ) {
        cerr << "Error initializing renderer: " << SDL_GetError() << endl;
    }

    Uint32 winId = SDL_GetWindowID ( newWindow );

    windows[winId] = new LE_Texture ( newWindow, newRenderer );
    return winId;
}

void LE_TextureManager::loadTexture ( Uint32 windowId, 
        std::string filePath, std::string textureId ) {
    SDL_Surface* tempSurface = IMG_Load ( filePath.c_str() );
    if ( tempSurface == nullptr ) {
        cerr << "Error loading image " << filePath << ": " << IMG_GetError() << endl;
        return;
    }

    auto it = windows.find( windowId );
    if ( it == windows.end() ) {
        SDL_FreeSurface( tempSurface );
        return;
    }

    LE_Texture* leText = it->second;

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface (
            leText->sdl_renderer,
            tempSurface
            );

    leText->addTexture ( textureId, newTexture );
}

void LE_TextureManager::addTile ( Uint32 windowId, std::string textureId,
                                  std::string tileId, int x, int y, int h, int w ) {
    auto it = windows.find( windowId );
    if ( it == windows.end() ) return;
    
    if ( h == 0 || w == 0 ) {
        SDL_Texture* texture = it->second->getTexture ( textureId );
        
        SDL_QueryTexture ( texture, NULL, NULL, &w, &h );   
    }
    it->second->addTile( tileId, new LE_Tile( textureId, x, y, h, w ) );
}

bool LE_TextureManager::draw ( Uint32 windowId, std::string tileId, int x, int y, double h, double w,
       bool scale, bool flipv, bool fliph, const double angle) {
    
    auto it = windows.find( windowId );
    if ( it == windows.end() ) return false;

    LE_Tile* tile = it->second->getTile ( tileId );
    SDL_Texture* texture = it->second->getTexture ( tile->textureId );

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if ( flipv ) flip = SDL_FLIP_VERTICAL;
    else if ( fliph ) flip = SDL_FLIP_HORIZONTAL;

    SDL_Rect src, dst;

    src.x = tile->x;
    src.y = tile->y;
    src.h = tile->h;
    src.w = tile->w;

    dst.x = x;
    dst.y = y;
    
    if ( scale ) {
        dst.w = src.w * w;
        dst.h = src.h * h;
    } else {
        dst.w = w;
        dst.h = h;
    }

    if ( 0 >
      SDL_RenderCopyEx( it->second->getRenderer(), texture, &src, &dst, angle, NULL, flip) ) {
        cerr << "Error rendering tile " << SDL_GetError() << endl;
        return false;
    }

    return true;
}
