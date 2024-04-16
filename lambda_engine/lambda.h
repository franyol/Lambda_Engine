#ifndef _LAMBDA_ENGINE_H_
#define _LAMBDA_ENGINE_H_

    #include "lambda_Game.h"
    #include "lambda_GameObject.h"
    #include "lambda_FSM.h"
    #include "lambda_InputHandler.h"
    #include "lambda_TextureManager.h"
    #include "lambda_AudioManager.h"
    #include "lambda_TileMap.h"
    #include "lambda_TextManager.h"

    bool LE_Init () {
        // Create singletons
        if ( !LE_TEXTURE->EverythingWasInit() ) return false;
        LE_GAME;
        LE_FSM;
        LE_INPUT;
        LE_TILEMAP;
        LE_AUDIO;
        LE_TEXT;

        return true;
    }

    void LE_Quit () {
        // Destroy singletons
        QUIT_LE_TEXT;
        QUIT_LE_AUDIO;
        QUIT_LE_GAME;
        QUIT_LE_FSM;
        QUIT_LE_INPUT;
        QUIT_LE_TILEMAP;
        QUIT_LE_TEXTURE;
    }

#endif
