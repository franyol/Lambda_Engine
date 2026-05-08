#ifndef _LAMBDA_ENGINE_MANY_TO_MANY_H_
#define _LAMBDA_ENGINE_MANY_TO_MANY_H_

#include "lambda_group_base.h"

class LE_ManyToMany: public LE_Group {
    friend class LE_GameState;

    public:

        /**
         * @brief Define an interaction betweeb game Objects A and B
         *
         * This method is run once for pair of game objects, so the
         * bejaviour of A and B must be equally defined independent of the order.
         * */
        virtual void interactorUpdateHandler(void* gameObj_A, void* gameObj_B) = 0;

        /**
         * @brief unused
         * */
        void objUpdateHandler(void *gameObj) override {}

        virtual void update() override;
};

#endif
