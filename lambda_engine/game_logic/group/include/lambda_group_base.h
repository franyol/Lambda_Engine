#ifndef _LAMBDA_ENGINE_GROUP_H_
#define _LAMBDA_ENGINE_GROUP_H_

class LE_GameObject;
class LE_GameState;

#include <map>
#include <string>

/**
 * @brief base class for groups
 *
 * Groups is a class that lets you define a common behaviour on a group of game objects
 * Useful when you have to share the same functionalities through several game objects
 * but don't want to deal with complex inheritance logic.
 * */
class LE_Group {
    friend class LE_GameState;

    protected:
        /**
         * @brief map containing LE_GameObjects belonging to the group
         * */
        std::map<std::string, LE_GameObject*> gameObjects;

        /**
         * @brief main object for one to many interactions
         * */
        LE_GameObject* mainObj;

        /*
         * @brief Group ID, this is defined when the object is
         * registered to a LE_State
         * */
        std::string id;

        /**
         * @brief Flag, while in false, the group updates won't be executed
         * */
        bool enabled;
    public:

        LE_Group();

        /**
         * @brief class destructor
         *
         * Calls to LE_Group::clean
         * */
        virtual ~LE_Group () { clean(); }

        /**
         * @brief returns if the group wants to be executed on the next game loop
         * */
        bool isEnabled ();

        /**
         * @brief enables the group
         * */
        void enable();

        /**
         * @brief disables the group
         * */
        void disable();

        /**
         * @brief Executes when a game object is registered to the group
         * */
        virtual void objRegisterHander(void* gameObj) {}

        /**
         * @brief Do an an action on all registered gameObject when updated
         * */
        virtual void objUpdateHandler(void* gameObj) = 0;

        /**
         * @brief Executes when a game Object is unregistered from the group
         * */
        virtual void objUnregisterHandler(void* gameObj) {}

        /**
         * @brief Executes on every game loop
         *
         * If not overwriten, will trigger objUpdateHander() on
         * every object registered in the group
         * */
        virtual void update () {
            for ( auto it = gameObjects.begin(); it != gameObjects.end(); ++it ) {
                void *gObj = it->second;
                objUpdateHandler(gObj);
            }
        }

        /**
         * @brief Set main game object for one to many interactors
         *
         * @param gameObj LE_GameObject* to be registered on the group
         * */
        void setMainObject (LE_GameObject* gameObj);

        /**
         * @brief Register a game object
         *
         * @param gameObj LE_GameObject* to be registered on the group
         * @param obgId   string with the object id
         * */
        void registerObject (LE_GameObject* gameObj, std::string objId);

        /**
         * @brief Unegister a game object
         *
         * @param obgId   string with the object id
         * */
        void unregisterObject ( std::string objId );

        /**
         * @brief Unegister a game object
         *
         * does not call unregister handler
         *
         * @param obgId   string with the object id
         * */
        void deletedObject ( std::string objId );

        /**
         * @brief get object by it's ID
         *
         * @param objId Game Object ID
         * @return LE_GameObject*
         * */
        LE_GameObject* getObject ( std::string objId );

        /**
         * @brief clears game objects
         * */
        virtual void clean () {
            gameObjects.clear();
        }
};

#endif
