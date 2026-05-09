#include <lambda.h>
#include <iostream>

using namespace std;

Uint32 mainWindow, secondWindow;

class SimpleGame : public LE_GameState {
    private:
        double dt;
        LE_Cursor* cursor;
        LE_Cursor* cursor2;

    public:
        void on_enter() {
            //LE_TEXTURE->loadFromXmlFile ( "test.xml", mainWindow );
            //LE_TILEMAP->loadFromXmlFile ( "test.xml", mainWindow );
            LE_Cursor::init( mainWindow );
            LE_Cursor::init( secondWindow );

            cursor = new LE_Cursor("black", mainWindow);
            cursor2 = new LE_Cursor("white", secondWindow);

            addObject(cursor, "cursor1");
            addObject(cursor2, "cursor2");
       }

        void update() {
            LE_GameState::update();

            if (LE_INPUT->getKeyState( SDL_SCANCODE_ESCAPE ) == keyState::pressed)
                LE_GAME->exit();
        }
};

int main ( int argc, char* argv[] ) {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    SDL_ShowCursor(SDL_DISABLE); // Hide the original cursor
                                 //
    // Create a window to show the texture
    mainWindow = LE_GAME->createWindow( "groups", 480, 640 );
    secondWindow = LE_GAME->createWindow( "groups", 480, 640 );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
