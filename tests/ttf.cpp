#include <lambda.h>
#include <string>

int main () {
    
    LE_Init();

    Uint32 mainWindow = LE_TEXTURE->createWindow( "Textbox", 480, 640 );

    LE_TEXT->loadFont( "Raleway-Black.ttf", 22, "myFont" );

    int lineSkip;
    int n;
    n = LE_TEXT->createTexture (
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla paria.",
            mainWindow,
            "TextTexture",
            "myFont",
            "a_",
            300,
            200,
            0,
            0,
            200,
            &lineSkip,
            nullptr
            );

    for ( int i = 0; i < n; i++ ) {
        LE_TEXTURE->draw ( mainWindow, "a_" + std::to_string(i),
               40, 40 + ( i*lineSkip ) );
    }
    LE_TEXTURE->present( mainWindow );

    SDL_Delay ( 5000 );

    LE_Quit();

    return 0;
}
