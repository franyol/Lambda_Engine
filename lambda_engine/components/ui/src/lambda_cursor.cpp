#include "lambda_cursor.h"
#include "lambda_TextureManager.h"
#include <config.h>

void LE_Cursor::init(Uint32 windowId) {
    LE_TEXTURE->loadFromXmlFile(std::string(LE_ASSET_DIR) + "/ui/cursor/cursor.xml", windowId);
    std::cout << std::string(LE_ASSET_DIR) << std::endl;
}

LE_Cursor::LE_Cursor() {}

LE_Cursor::LE_Cursor(std::string variant): variant(std::move(variant)) {}

LE_Cursor::~LE_Cursor() {}

void LE_Cursor::setup() {
    h = 50;
    w = 50;
    scale = true;
    currentFrame = variant;
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
