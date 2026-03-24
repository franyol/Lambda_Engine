#include "lambda_group_base.h"
#include "lambda_GameObject.h"

LE_Group::LE_Group() {
    enabled = true;
}

void LE_Group::registerObject ( LE_GameObject* gameObj, std::string objId ) {
    objRegisterHander(gameObj);
    gameObjects[objId] = gameObj;
    gameObj->addGroup(this, id);
}

void LE_Group::unregisterObject ( std::string objId ) {
    auto it = gameObjects.find(objId);
    if (it != gameObjects.end()) {
        objUnregisterHandler( it->second );
        it->second->popGroup(id);
        gameObjects.erase(it);
    }
}

void LE_Group::deletedObject ( std::string objId ) {
    gameObjects.erase(objId);
}

LE_GameObject* LE_Group::getObject ( std::string objId ) {
    auto it = gameObjects.find(objId);
    if (it != gameObjects.end()) {
        return it->second;
    }
    return nullptr;
}

bool LE_Group::isEnabled () { return enabled; }

void LE_Group::enable () { enabled = true; }

void LE_Group::disable () { enabled = false; }
