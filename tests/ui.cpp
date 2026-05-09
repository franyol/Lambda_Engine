#include <lambda.h>
#include <iostream>

using namespace std;

Uint32 mainWindow;

class SimpleGame : public LE_GameState {
    private:
        double dt;
        LE_Cursor* cursor;

    public:
        void on_enter() {
            LE_TEXTURE->loadFromXmlFile ( "test.xml", mainWindow );
            LE_TILEMAP->loadFromXmlFile ( "test.xml", mainWindow );
            LE_Cursor::init( mainWindow );

            cursor = new LE_Cursor();

            addObject(cursor, "cursor");
       }

        void update() {
            LE_GameState::update();

            if (LE_INPUT->getKeyState( SDLK_ESCAPE ) == keyState::pressed)
                LE_GAME->exit();
        }
};

int main ( int argc, char* argv[] ) {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    // Create a window to show the texture
    mainWindow = LE_GAME->createWindow( "groups", 480, 640 );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
