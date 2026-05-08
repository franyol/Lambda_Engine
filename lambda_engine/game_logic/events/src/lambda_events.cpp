#include "lambda_events.h"
#include <iostream>

LE_EventBus::LE_EventBus() {}

LE_EventBus::~LE_EventBus() {}

void LE_EventBus::subscribe(Callback cb, std::string listenerId) {
    listeners[listenerId] = cb;
}

void LE_EventBus::unsubscribe(std::string listenerId) {
    auto it = listeners.find(listenerId);
    if (it != listeners.end()) {
        listeners.erase(it);
    }
}

void LE_EventBus::emit(void* eventData) {
    for (auto it = listeners.begin(); it != listeners.end(); it++) {
        Callback cb = it->second;
        cb(eventData);
    }
}

LE_Events::LE_Events() {}

LE_Events::~LE_Events() { clean(); }

LE_Events* LE_Events::Instance ( void ) {
    if ( the_instance == nullptr ) {
        the_instance = new LE_Events();
    }
    return the_instance;
}

void LE_Events::destroyInstance () {
    if ( the_instance != nullptr ) {
        delete the_instance;
        the_instance = nullptr;
    }
}

LE_EventBus* LE_Events::registerEventBus (std::string busId) {
    auto it = eventBuses.find ( busId );
    if ( it != eventBuses.end() ) {
        return it->second;
    }

    LE_EventBus* new_bus = new LE_EventBus();
    eventBuses[busId] = new_bus;
    return new_bus;
}

void LE_Events::uregisterEventBus (std::string busId) {
    eventBuses.erase ( busId );
}

void LE_Events::registerCallback (std::string busId, Callback cb, std::string listenerId) {
    LE_EventBus* bus = registerEventBus(busId);

    bus->subscribe(cb, listenerId);
}

void LE_Events::emit (std::string busId, void* eventData) {
    LE_EventBus* bus = getEventBus(busId);
    if (bus != nullptr) {
        bus->emit(eventData);
    }
}

LE_EventBus* LE_Events::getEventBus (std::string busId) {
    auto it = eventBuses.find(busId);

    if (it != eventBuses.end()) {
        return it->second;
    }

    return nullptr;
}

void LE_Events::clean () {
    for (auto it = eventBuses.begin(); it != eventBuses.end(); it++) {
        delete it->second;
    }
    eventBuses.clear();
}
