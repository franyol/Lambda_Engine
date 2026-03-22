#include "lambda_TileMap.h"
#include "lambda_TextureManager.h"
#include "lambda_XMLFabric.h"
#include <iostream>

using namespace rapidxml;

// Define static members
std::map<std::string, LE_TileMap*> LE_TileMapManager::projectMaps;
LE_TileMapManager* LE_TileMapManager::the_instance;

void LE_TileMap::drawMap () {
    if ( !LE_TEXTURE->EverythingWasInit() ) {
        std::cerr << "ERROR: Texture Manager was not initialized, can't draw map" 
            << std::endl;
        return;
    }
    for ( auto it = draws.begin(); it != draws.end(); it++ ) {
        std::string tileId = it->first;
        std::vector<LE_TileDrawInfo*> drawInfoV = it->second;

        for ( LE_TileDrawInfo* drawInfo : drawInfoV ) {
            LE_TEXTURE->draw ( windowId, tileId, drawInfo->x, drawInfo->y,
                   drawInfo->h, drawInfo->w, drawInfo->scale, drawInfo->flipv,
                   drawInfo->fliph, drawInfo->angle );
        }
    }
}

void LE_TileMap::blendToTexture ( std::string textureId ) {

    // Calculate the width and height of the tile map
    int x_start, x_end, y_start, y_end;
    bool first = true;

    for ( auto it = draws.begin(); it != draws.end(); it++ ) {
        std::string tileId = it->first;
        std::vector<LE_TileDrawInfo*> drawInfoV = it->second;

        int tile_h, tile_w, src_h, src_w;

        LE_TEXTURE->getTileSize ( windowId, tileId, &src_h, &src_w );

        for ( LE_TileDrawInfo* drawInfo : drawInfoV ) {
            // Get tile drawn dimentions
            if ( drawInfo->scale ) {
                tile_h = drawInfo->h * src_h;
                tile_w = drawInfo->w * src_w;
            } else {
                tile_h = drawInfo->h;
                tile_w = drawInfo->w;
            }

            if ( first ) {
                first = false;
                x_start = drawInfo->x;
                y_start = drawInfo->y;
                x_end = drawInfo->x + tile_w;
                y_end = drawInfo->y + tile_h;
            } else {
                if ( drawInfo->x < x_start ) x_start = drawInfo->x;
                if ( drawInfo->y < y_start ) y_start = drawInfo->y;
                if ( drawInfo->x + tile_w > x_end ) x_end = drawInfo->x + tile_w;
                if ( drawInfo->y + tile_h > y_end ) y_end = drawInfo->y + tile_h;
            }
        }
    }

    int w = x_end - x_start;
    int h = y_end - y_start;

    LE_TEXTURE->createTargetTexture ( windowId, textureId, h, w ); 

    // Draw map in origin positions
    moveMap ( -x_start, -y_start );
    drawMap ();

    // Restore map and render target
    moveMap ( x_start, y_start );
    LE_TEXTURE->restoreRenderTarget ( windowId );
}

void tilemap_onRead ( const Attr& attr, const std::string value ) {
    LE_TILEMAP->addMap ( attr.at("id"), new LE_TileMap ( stoi(attr.at("windowId")) ) );
}
void pos_onRead ( const Attr& attr, const std::string value ) {
    LE_TILEMAP->addDrawInfo ( attr.at("id"), attr.at("tile"),
           new LE_TileDrawInfo ( 
               stoi(attr.at("x")),
               stoi(attr.at("y")),
               stod(attr.at("h")),
               stod(attr.at("w")),
               stod(attr.at("angle")),
               attr.at("scale") == "true",
               attr.at("flipv") == "true",
               attr.at("fliph") == "true"
               ) );
}

void LE_TileMapManager::loadFromXmlFile ( std::string filePath, Uint32 windowId ) {

    LE_XMLNode mainNode ( "TILEMAPS" ), 
               tilemapN ( "tilemap" ),
               setN ( "set" ),
               posN ( "pos" );

    Attr attr;
    attr["windowId"] = std::to_string(windowId);

    tilemapN.setOnRead ( tilemap_onRead );
    posN.setOnRead ( pos_onRead );

    setN.addChild ( &posN );
    tilemapN.addChild ( &setN );
    mainNode.addChild ( &tilemapN );

    mainNode.readDoc ( filePath, &attr );

}
