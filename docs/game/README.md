# LE_Game

## Table of Contents
- [Initialize LE_GAME](#initialize-le_game)
- [Managing windows](#managing-windows)
- [Fixing a Framerate](#fixing-a-framerate)
- [The main loop](#the-main-loop)
  - [1. Handle events](#1-handle-events)
  - [2. Update](#2-update)
  - [3. Render](#3-render)
- [Usage example](#usage-example)

---

This class is the heart of Lambda Engine; it contains the entry point of any game.

Like many other classes, `LE_Game` follows the Singleton design pattern. You can access its instance through `LE_GAME`, which is a shortcut for `LE_Game::Instance()`.

## Initialize LE_GAME

The required initialization for `LE_GAME` is performed by calling:

```cpp
LE_init();
```

Remember to deinitialize everything before closing the application:

```cpp
LE_Quit();
```

## Managing windows

Lambda Engine allows you to work with multiple windows. You can create a window using the `createWindow` method. This function returns a window ID that you can later use to manage where your game objects are rendered.
```cpp
/**
 * @brief Creates a new window
 *
 * @param title window title
 * @param w window width in pixels
 * @param h window height in pixels
 * @param full_screen Expand the window to fill the screen
 * @param input_focus Sets the focus to this window
 * @param borderless Eliminates the window borderw (and close button)
 * @param resizable User can resize the window
 * @param gpu Tell SDL to use gpu when available
 *
 * @return Window ID
 * */
Uint32 createWindow ( const char* title, int w, int h,
        bool full_screen = false, bool input_focus = false,
        bool hidden = false, bool borderless = false,
        bool resizable = false);
```

If you need more control over your windows creation, you can also create your window as you wish with `addWindow`, which receives an SDL_Window object:

```cpp
/**
 * @brief add an existing window
 *
 * @return window ID
 * */
Uint32 addWindow ( SDL_Window* newWindow );
```

Both these methods then call the TextureManager which creates a LE_Window object and a renderer created with SDL_RENDERER_ACCELERATED option enabled, associated to that window.

## Fixing a Framerate

Framerate affects how fast the game main loop runs. By default, it is unfixed, meaning the game updates as fast as the machine allows. To set a target framerate, use the `fixFramerate` method.

## The main loop

`LE_GAME->mainLoop()` is the entry point that runs all the game logic. It also calculates the delta time between loop iterations, which is necessary for consistent game logic.

The delta time is available anywhere in the project via:

```cpp
LE_GAME->getDeltaTime();
```

This returns the elapsed time since the last update in milliseconds.

To stop the game loop and ensure everything is cleaned properly, use:

```cpp
LE_GAME->setRunning(false);
```

The game will stop after the next loop iteration.

Each iteration of the main loop performs the following actions in this exact order:

### 1. Handle events

Runs `LE_INPUT->update()`, meaning the game records all input events at this point.

### 2. Update

Executes `LE_FSM->update()`, which calls the `update()` method of the current state in the FSM queue.

### 3. Render

During rendering, all windows are cleared with a white background. Then, `LE_FSM->render()` is called to render every state in the queue. This creates the effect of stacked states, where only the top state is updated while others remain visible but paused.

Finally, Lambda Engine instructs SDL to update all windows created with the `LE_GAME->createWindow` method.

## Usage example

```cpp
int main () {
    if ( !LE_Init() ) {
        cerr << "Could not initialize Lambda Engine" << endl;
    }

    Uint32 mainWindow = LE_GAME->createWindow ( "my game", SCREEN_W, SCREEN_H, true );

    LE_GAME->fixFramerate ( 60 );
    LE_FSM->push_back ( new SimpleGame() );

    LE_GAME->mainLoop();

    LE_Quit();
    return 0;
}
```
