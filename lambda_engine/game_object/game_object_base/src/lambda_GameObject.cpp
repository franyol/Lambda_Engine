#include "lambda_GameObject.h"
#include "lambda_FSM.h"

void LE_GameObject::addGroup ( LE_Group* newGroup, std::string groupId ) {
    groups[groupId] = newGroup;
}

void LE_GameObject::popGroup ( std::string groupId ) {
    groups.erase(groupId);
}

void LE_GameObject::unregisterGroup ( std::string groupId ) {
    LE_Group* gr = getGroup(groupId);
    if (gr) {
        gr->unregisterObject(id);
    }
}

void LE_GameObject::registerGroup ( std::string groupId ) {
    LE_GameState *currentState = LE_FSM->getCurrentState();
    LE_Group* gr = currentState->getGroup(groupId);
    if (gr) {
        gr->registerObject(this, this->id);
    }
}

LE_Group* LE_GameObject::getGroup ( std::string groupId ) {
    auto it = groups.find(groupId);
    if (it != groups.end()) {
        return it->second;
    }
    return nullptr;
}

void LE_GameObject::enableGroup ( std::string groupId ) {
    LE_Group* gr = getGroup(groupId);
    if (gr) {
        gr->enable();
    }
}

void LE_GameObject::disableGroup ( std::string groupId ) {
    LE_Group* gr = getGroup(groupId);
    if (gr) {
        gr->disable();
    }
}
