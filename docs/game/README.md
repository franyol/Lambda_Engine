# LE_Game

This is class is the heart of Lambda Engine, it carries the entrypoint of any game.

As many other classes, LE_Game follows the Singleton design pattern. You can acces the LE_Game instance by accessing to `LE_GAME`, which is a shortcut for `LE_Game::Instance()`.

## Initialize LE_GAME:

The necessary initialization for LE_GAME will be done after calling:

```cpp
LE_init();
```

Remember to deinitializate everything before closing the application:

```cpp
LE_Quit();
```

## Managing windows

Lambda Engine lets you work over several windows, you can create a window by using the `createWindow` method, this function will return the window ID you can use to later manage in which window you will render your game objects.


## Fixing a Framerate

Framerate affects on how fast will the game main loop run, by default, it will be unfixed, making the game update as fast as the machine lets it, to fix a target framerate, use the `fixFramerate` method.


## The main loop

`LE_GAME->mainLoop()` is the entrypoint that will put all the logic behind the game to run, besides this, this method also calculates the deltaTime elapsed between gameLoop laps which is necessary to program the game logic. The deltatime is available along any project by using: `LE_GAME->getDeltaTime();`, which returns the elapsed time since the last update in milliseconds.

To stop the game loop and ensure everything is cleaned propperly, use the method:
```cpp
LE_GAME->setRunning = false;
```
At any point, the game will be finalized after the next lap.

Each lap inside the mainLoop does the following actions in this exact order:

### 1. Handle events

Runs `LE_INPUT->update()`, which means, the game will record every button press at this point.

### update

The `LE_FSM->update()` method is executed, this will run the update() method of the current state in the LE_FSM queue.

### Render

During the render routine, all windows will be cleaned up with white filling the entire windows for every window. After that, a call to `LE_FSM->render()` is done to render every state in the queue; this will give the effect that the stacked states are paused behind the current state which is the only one being updated at the moment.

At the end, Lambda Engine instruct SDL to update every window opened using the `LE_GAME-createWindow` method.


## Usage example:


```cpp
int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    mainWindow = LE_GAME->createWindow ( "my game", SCREEN_W, SCREEN_H, true );
    LE_TEXTURE->createTargetTexture ( mainWindow, "mainView", SCREEN_H, SCREEN_W );
    LE_TEXTURE->addTile ( mainWindow, "mainView", "mainViewTile" );
    LE_TEXTURE->getWindowSize ( mainWindow, &windowHeight, &windowWidth );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
```
