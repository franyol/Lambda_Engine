#include <lambda.h>
#include <iostream>

using namespace std;

Uint32 mainWindow;

class BaseObj : public LE_GameObject {
    friend class Physics;
    friend class Hitboxes;

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

            if (gObj->x > 540) gObj->x_speed = -gObj->x_speed;
            if (gObj->y > 380) gObj->y_speed = -gObj->y_speed;
            if (gObj->x < 0) gObj->x_speed = -gObj->x_speed;
            if (gObj->y < 0) gObj->y_speed = -gObj->y_speed;
        }
};

class Hitboxes : public LE_ManyToMany {
    public:
        void interactorUpdateHandler(void* gameObj_A, void* gameObj_B) override {
            BaseObj* a = (BaseObj*) gameObj_A;
            BaseObj* b = (BaseObj*) gameObj_B;

            int a_left   = a->x;
            int a_right  = a->x + a->w;
            int a_top    = a->y;
            int a_bottom = a->y + a->h;

            int b_left   = b->x;
            int b_right  = b->x + b->w;
            int b_top    = b->y;
            int b_bottom = b->y + b->h;

            bool collision =
                a_left < b_right  &&
                a_right > b_left  &&
                a_top < b_bottom  &&
                a_bottom > b_top;

            if (!collision)
                return;

            // centers
            float a_center_x = (a_left + a_right) * 0.5f;
            float a_center_y = (a_top + a_bottom) * 0.5f;

            float b_center_x = (b_left + b_right) * 0.5f;
            float b_center_y = (b_top + b_bottom) * 0.5f;

            // distance between centers
            float dx = b_center_x - a_center_x;
            float dy = b_center_y - a_center_y;

            // overlap amount
            float overlap_x =
                (a->w * 0.5f + b->w * 0.5f) - std::abs(dx);

            float overlap_y =
                (a->h * 0.5f + b->h * 0.5f) - std::abs(dy);

            if (overlap_x < overlap_y) {
                // horizontal collision
                int temp = a->x_speed;
                a->x_speed = b->x_speed;
                b->x_speed = temp;
            } else {
                // vertical collision
                int temp = a->y_speed;
                a->y_speed = b->y_speed;
                b->y_speed = temp;
            }
        }

        void objRegisterHander(void* gameObj) override {}
};

class GO1: public LE_GameObject {
    friend class SimpleGame;

    protected:
        int x_speed;
        int y_speed;

    public:

        GO1 (int x_, int y_) {
            setposition(x_, y_);
        }

        void setup () {
            frames["frame"] = { "im1_tile", mainWindow };
            currentFrame = "frame";
            x_speed = 120;
            y_speed = 30;
            w = 100;
            h = 100;
            scale = false;
            registerGroup("physics");
            registerGroup("hitboxes");
        }

        void setposition(int x_, int y_) {
            x = x_;
            y = y_;
        }
};

class GO2: public LE_GameObject {
    protected:
        int x_speed;
        int y_speed;

    public:
        void setup () {
            frames["frame"] = { "im2_tile", mainWindow };
            currentFrame = "frame";
            x_speed = 100;
            y_speed = 70;
            x = 130;
            y = 130;
            w = 100;
            h = 100;
            scale = false;

            registerGroup("physics");
            registerGroup("hitboxes");
        }
};

class SimpleGame : public LE_GameState {
    private:
        double dt;
        GO1* gameObject1;
        GO2* gameObject2;
        GO1* gameObject3;

    public:
        void on_enter() {
            gameObject1 = new GO1(30, 30);
            gameObject2 = new GO2();

            LE_TEXTURE->loadFromXmlFile ( "test.xml", mainWindow );
            LE_TILEMAP->loadFromXmlFile ( "test.xml", mainWindow );

            addGroup(new Physics(), "physics");
            addGroup(new Hitboxes(), "hitboxes");

            addObject(gameObject1, "go1");
            addObject(gameObject2, "go2");

            gameObject3 = new GO1(300, 200);
            addObject(gameObject3, "go3");
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
