#include <lambda.h>
#include <iostream>

using namespace std;

Uint32 mainWindow;

class BaseObj : public LE_GameObject {
    friend class Physics;

    protected:
    int x_speed;
    int y_speed;
};

class Physics : public LE_Group {
    public:
        void objUpdateHandler(void *gameObj) override {
            BaseObj* gObj = (BaseObj*) gameObj;
            double dt = LE_GAME->getDeltaTime();

            gObj->x += gObj->x_speed*dt/1000;
            gObj->y += gObj->y_speed*dt/1000;

            if (gObj->x > 540) gObj->x_speed = -50;
            if (gObj->y > 380) gObj->y_speed = -50;
            if (gObj->x < 0) gObj->x_speed = 50;
            if (gObj->y < 0) gObj->y_speed = 50;
        }
};

class GO1: public LE_GameObject {
    protected:
        int x_speed;
        int y_speed;

    public:

        void setup () {
            LE_GameObject::setup();
            frames["frame"] = { "im1_tile", mainWindow };
            currentFrame = "frame";
            x_speed = 50;
            y_speed = 0;
            x = 10;
            y = 10;
            w = 100;
            h = 100;
            scale = false;

            registerGroup("physics");
        }
};

class GO2: public LE_GameObject {
    protected:
        int x_speed;
        int y_speed;

    public:
        void setup () {
            LE_GameObject::setup();
            frames["frame"] = { "im2_tile", mainWindow };
            currentFrame = "frame";
            x_speed = 0;
            y_speed = 50;
            x = 100;
            y = 100;
            w = 100;
            h = 100;
            scale = false;

            registerGroup("physics");
        }
};

class SimpleGame : public LE_GameState {
    private:
        double dt;
        GO1* gameObject1;
        GO2* gameObject2;

    public:
        void on_enter() {
            gameObject1 = new GO1();
            gameObject2 = new GO2();

            LE_TEXTURE->loadFromXmlFile ( "test.xml", mainWindow );
            LE_TILEMAP->loadFromXmlFile ( "test.xml", mainWindow );

            addGroup(new Physics(), "physics");

            addObject(gameObject1, "go1");
            addObject(gameObject2, "go2");
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
