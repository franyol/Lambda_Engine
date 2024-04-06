#include <iostream>

#ifndef _LAMBDA_ENGINE_H_
#define _LAMBDA_ENGINE_H_

    #include "game_logic/lambda_Game.h"
    #include "game_object/lambda_GameObject.h"
    #include "game_logic/lambda_FSM.h"
    #include "input_handler/lambda_InputHandler.h"
    #include "sdl_wrappers/lambda_TextureManager.h"
    #include "sdl_wrappers/lambda_AudioManager.h"
    #include "visual/lambda_TileMap.h"

    bool LE_Init () {
        // Create singletons
        if ( !LE_TEXTURE->EverythingWasInit() ) return false;
        LE_GAME;
        LE_FSM;
        LE_INPUT;
        LE_TILEMAP;
        LE_AUDIO;

        return true;
    }

    void LE_Quit () {
        // Destroy singletons
        QUIT_LE_AUDIO;
        QUIT_LE_GAME;
        QUIT_LE_FSM;
        QUIT_LE_INPUT;
        QUIT_LE_TILEMAP;
        QUIT_LE_TEXTURE;
    }

#endif
