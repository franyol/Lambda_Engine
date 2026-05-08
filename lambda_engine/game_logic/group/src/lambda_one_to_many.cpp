#include "lambda_one_to_many.h"

void LE_OneToMany::update() {
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {
        interactorUpdateHandler(mainObj, it->second);
    }
}

