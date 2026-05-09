#include "lambda_cursor.h"
#include "lambda_TextureManager.h"
#include <lambda_Game.h>
#include <lambda_config.h>

void LE_Cursor::init(Uint32 windowId) {
    LE_TEXTURE->loadFromXmlFile(std::string(LE_ASSET_DIR) + "/ui/cursor/cursor.xml", windowId);
}

LE_Cursor::LE_Cursor(Uint32 window): window(window) {}

LE_Cursor::LE_Cursor(std::string variant, Uint32 window):
    variant(std::move(variant)),
    window(window)
{}

LE_Cursor::~LE_Cursor() {}

void LE_Cursor::setup() {
    h = 50;
    w = 50;
    scale = false;
    currentFrame = variant;

    frames["black"] = { "black", window };
    frames["black-click"] = { "black-click", window };
    frames["white"] = { "white", window };
    frames["white-click"] = { "white-click", window };

    addEventHandler(
            "on_click",
            [this](void* eventData) { on_click(); } );
    addEventHandler(
            "on_idle",
            [this](void* eventData) { on_idle(); } );
    addEventHandler(
            "on_hover",
            [this](void* eventData) { on_hover(); } );
}

void LE_Cursor::update() {
    MouseInput* mouse = LE_INPUT->getMouse();

    if (mouse->windowId != window) return;

    x = mouse->xabs;
    y = mouse->yabs;

    switch (mouse->left) {
        case keyState::pressed:
            if (state != cursorState::clicked) {
                emitEvent("on_click", nullptr);
            }
            break;
        case keyState::released:
            emitEvent("on_idle", nullptr);
            break;
        default:
            break;
    }
}

void LE_Cursor::on_idle() {
    currentFrame = variant;
}

void LE_Cursor::on_click() {
    currentFrame = variant + "-click";
}

void LE_Cursor::on_hover() {
    // currentFrame = ""; Not defined
}
