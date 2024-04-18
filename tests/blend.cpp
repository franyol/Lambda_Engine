#include <lambda.h>
#include <iostream>

int main ( int argc, char* argv[] ) {
    LE_Init();
    // Create a window to show the texture
    Uint32 mainWindow = LE_TEXTURE->createWindow( "Blend", 480, 640 );

    // Load two images into the texture manager
    LE_TEXTURE->loadTexture( mainWindow, "myimage1.png", "im1" );
    LE_TEXTURE->loadTexture( mainWindow, "myimage2.png", "im2" );
    // Create tiles for those textures
    LE_TEXTURE->createTile( mainWindow, "im1", "im1_tile" );
    LE_TEXTURE->createTile( mainWindow, "im2", "im2_tile" );

    // Create a new texture and tile to blend the tiles:
    LE_TEXTURE->createTargetTexture( mainWindow, "im3", 480, 640 );
    LE_TEXTURE->createTile( mainWindow, "im3", "im3_tile" );

    // Now when we draw, we draw into the texture instead of the window
    LE_TEXTURE->setBlendMode( LE_BlendMode::add, mainWindow, "im2" );
    LE_TEXTURE->draw( mainWindow, "im1_tile", 0, 0, 480, 640, false );
    LE_TEXTURE->draw( mainWindow, "im2_tile", 150, 150, 240, 320, false );

    // Restore the target to the main window
    LE_TEXTURE->restoreRenderTarget( mainWindow );


    LE_TILEMAP->loadFromXmlFile ( "tests/test.xml", mainWindow );

    LE_TILEMAP->drawMap ( "myMap" );

    // display the blended texture into the window
    // LE_TEXTURE->draw( mainWindow, "im3_tile", 0, 0 );
    LE_TEXTURE->present( mainWindow );

    // Mantain the window during 5 seconds
    SDL_Delay ( 5000 );
    LE_Quit();
    return 0;
}
