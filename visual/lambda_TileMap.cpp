#include "lambda_TileMap.h"
#include "rapidxml/rapidxml.hpp"
#include "sdl_wrappers/lambda_TextureManager.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

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

void LE_TileMapManager::loadFromXmlFile ( std::string filePath, Uint32 windowId ) {
    std::ifstream file ( filePath.c_str() );
    std::string xmlString(
            (std::istreambuf_iterator<char>(file)), 
            std::istreambuf_iterator<char>());

    xml_document<> doc;
    doc.parse<0>(&xmlString[0]);

    xml_node<>* currentNode; 
    xml_node<>* childNode;
    xml_node<>* childSecond;
    xml_node<>* childThird;
    currentNode = doc.first_node();

    if ( std::strcmp(currentNode->name(), "TEXTURELOADER") != 0 ) {
        std::cerr << "Invalid xml format " << filePath << std::endl;
        std::cerr << "Expected TEXTURELOADER node, but got: " 
            << currentNode->name() << std::endl;
    }

    currentNode = currentNode->first_node();

    while ( currentNode ) {


        // Looking for TILESETS and TILEMAPS
        
        if ( std::strcmp( currentNode->name(), "TILESETS" ) == 0 ) {
            childNode = currentNode->first_node();
            while ( childNode ) {
                // Looking for texture
                if ( std::strcmp( childNode->name(), "texture" ) == 0 ) {
                
                    std::string textureId = "";
                    std::string filename = "";
                    // Parse attributes
                    for ( xml_attribute<>* attr = childNode->first_attribute(); attr; 
                            attr = attr->next_attribute() ) {
                        if ( std::strcmp (attr->name(), "filename") == 0 ) {
                            filename = attr->value();
                        }
                        else if ( std::strcmp (attr->name(), "id") == 0 ) {
                            textureId = attr->value();
                        }
                    }

                    // If attributes are missing
                    if ( textureId == "" || filename == "" ) {
                        childNode = childNode->next_sibling();
                        continue;
                    }

                    // All set, create the texture
                    LE_TEXTURE->loadTexture( windowId, filename, textureId );

                    childSecond = childNode->first_node();
                    while ( childSecond ) {
                        // Looking for tile

                        if ( std::strcmp( childSecond->name(), "tile" ) == 0 ) {
                            std::string tileId = "";
                            int x, y;
                            int h, w;
                            bool xb, yb, hb, wb;
                            xb = yb = hb = wb = false;
                            // Parse attributes
                            for ( xml_attribute<>* attr = childSecond->first_attribute(); attr; 
                                    attr = attr->next_attribute() ) {
                                if ( std::strcmp (attr->name(), "id") == 0 ) {
                                    tileId = attr->value();
                                }
                                else if ( std::strcmp (attr->name(), "x") == 0 ) {
                                    xb = true;
                                    x = std::atoi(attr->value());
                                }
                                else if ( std::strcmp (attr->name(), "y") == 0 ) {
                                    yb = true;
                                    y = std::atoi(attr->value());
                                }
                                else if ( std::strcmp (attr->name(), "w") == 0 ) {
                                    wb = true;
                                    w = std::atoi(attr->value());
                                }
                                else if ( std::strcmp (attr->name(), "h") == 0 ) {
                                    hb = true;
                                    h = std::atoi(attr->value());
                                }
                            }

                            // If attributes are missing
                            if ( tileId == "" || !xb || !yb || !hb || !wb )  {
                                childSecond = childSecond->next_sibling();
                                continue;
                            }

                            // All set, create the tile
                            LE_TEXTURE->addTile( windowId, textureId, tileId, x, y, h, w );
                        }
                    
                        childSecond = childSecond->next_sibling();
                        }
                    }
                childNode = childNode->next_sibling();
            }
        } 
        
        else if ( std::strcmp( currentNode->name(), "TILEMAPS" ) == 0 ) {
            childNode = currentNode->first_node();
            while ( childNode ) {
                // Looking for tilemap
                if ( std::strcmp( childNode->name(), "tilemap" ) == 0 ) {

                    std::string mapId = "";
                    // Parse attributes
                    for ( xml_attribute<>* attr = childNode->first_attribute(); attr; 
                            attr = attr->next_attribute() ) {
                        if ( std::strcmp (attr->name(), "id") == 0 ) {
                            mapId = attr->value();
                        }
                    }

                    // If attributes are missing
                    if ( mapId == "" ) {
                        childNode = childNode->next_sibling();
                        continue;
                    }
               
                    // All set, create new map
                    addMap ( mapId, new LE_TileMap( windowId ) );

                    childSecond = childNode->first_node();
                    while ( childSecond ) {
                        // Looking for set
                        if ( std::strcmp( childSecond->name(), "set" ) == 0 ) {
                    
                            std::string tileId = "";
                            // Parse attributes
                            for ( xml_attribute<>* attr = childSecond->first_attribute(); attr; 
                                    attr = attr->next_attribute() ) {
                                if ( std::strcmp (attr->name(), "tile") == 0 ) {
                                    tileId = attr->value();
                                }
                            }

                            // If attributes are missing
                            if ( tileId == "" ) {
                                childSecond = childSecond->next_sibling();
                                continue;
                            }

                            childThird = childSecond->first_node();
                            while ( childThird ) {
                                // Looking for pos
                                
                                if ( std::strcmp( childThird->name(), "pos" ) == 0 ) {

                                    int x, y;
                                    double h, w, angle;
                                    bool scale, flipv, fliph;
                                    bool xb, yb, hb, wb, scaleb, flipvb, fliphb, angleb;
                                    xb = yb = hb = wb = angleb = 
                                        scaleb = flipvb = fliphb = false;
                                    // Parse attributes
                                    for ( xml_attribute<>* attr = childThird->first_attribute(); attr; 
                                            attr = attr->next_attribute() ) {
                                        if ( std::strcmp (attr->name(), "angle") == 0 ) {
                                            angleb = true;
                                            angle = std::atof(attr->value());
                                        }
                                        else if ( std::strcmp (attr->name(), "x") == 0 ) {
                                            xb = true;
                                            x = std::atoi(attr->value());
                                        }
                                        else if ( std::strcmp (attr->name(), "y") == 0 ) {
                                            yb = true;
                                            y = std::atoi(attr->value());
                                        }
                                        else if ( std::strcmp (attr->name(), "w") == 0 ) {
                                            wb = true;
                                            w = std::atof(attr->value());
                                        }
                                        else if ( std::strcmp (attr->name(), "h") == 0 ) {
                                            hb = true;
                                            h = std::atof(attr->value());
                                        }
                                        else if ( std::strcmp (attr->name(), "scale") == 0 ) {
                                            scaleb = true;
                                            scale = std::strcmp(attr->value(), "true") == 0;
                                        }
                                        else if ( std::strcmp (attr->name(), "flipv") == 0 ) {
                                            flipvb = true;
                                            flipv = std::strcmp(attr->value(), "true") == 0;
                                        }
                                        else if ( std::strcmp (attr->name(), "fliph") == 0 ) {
                                            fliphb = true;
                                            fliph = std::strcmp(attr->value(), "true") == 0;
                                        }
                                    }

                                    // If attributes are missing
                                    if ( !xb || !yb || !hb || !wb
                                        || !angleb || !scaleb || !flipvb || !fliphb )  {
                                        childThird = childThird->next_sibling();
                                        continue;
                                    }

                                    // All set, add the tile to the map
                                    addDrawInfo ( mapId, tileId, new LE_TileDrawInfo (
                                                x, y, h, w, angle, scale, flipv, fliph
                                                ) );
                                }
                            
                                childThird = childThird->next_sibling();
                            }   
                        }
                    
                        childSecond = childSecond->next_sibling();
                    }
                }
                childNode = childNode->next_sibling();
            }
        }
       
        else {
            std::cerr << "Warning: Unknown node name: " << currentNode->name() << std::endl;
        }

        currentNode = currentNode->next_sibling();
    }
}
