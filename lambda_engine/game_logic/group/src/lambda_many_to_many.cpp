#include "lambda_many_to_many.h"

void LE_ManyToMany::update() {
    for (auto it1 = gameObjects.begin(); it1 != gameObjects.end(); ++it1) {
        void* a = it1->second;

        auto it2 = it1;
        ++it2;

        for (; it2 != gameObjects.end(); ++it2) {
            void* b = it2->second;

            interactorUpdateHandler(a, b);
        }
    }
}
