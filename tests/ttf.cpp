#include <lambda.h>
#include <string>

int main () {
    
    LE_Init();

    Uint32 mainWindow = LE_TEXTURE->createWindow( "Textbox", 480, 640 );

    LE_TEXT->loadFont( "Raleway-Black.ttf", 10, "myFont" );

    int lineSkip;
    int n;
    n = LE_TEXT->createTexture (
            "Hello friends! Let's test our text output together.",
            mainWindow,
            "TextTexture",
            "myFont",
            "a_",
            50,
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
