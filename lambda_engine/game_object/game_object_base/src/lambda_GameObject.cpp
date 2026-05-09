#include "lambda_GameObject.h"
#include "lambda_FSM.h"

LE_GameObject::LE_GameObject()
    : x(0), y(0), h(1), w(1),
      angle(0),
      scale(true),
      flipv(false),
      fliph(false),
      destroy_me(false)
{}

void LE_GameObject::addEventHandler ( std::string name, Callback cb ) {
    LE_EVENTS->registerCallback(this->id + name, cb, this->id);
    this->registeredBuses.push_back(this->id + name);
}

void LE_GameObject::emitEvent ( std::string name, void* eventData ) {
    LE_EventBus* bus = LE_EVENTS->getEventBus(this->id + name);
    if (bus) {
        bus->emit(eventData);
    }
}

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

void LE_GameObject::setAsGroupMainObj ( std::string groupId ) {
    LE_GameState *currentState = LE_FSM->getCurrentState();
    LE_Group* gr = currentState->getGroup(groupId);
    if (gr) {
        gr->setMainObject(this);
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
