#ifndef _LAMBDA_EVENTS_H_
#define _LAMBDA_EVENTS_H_

#include <map>
#include <vector>
#include <string>
#include <functional>

#define LE_EVENTS LE_Events::Instace()

#define QUIT_LE_EVENTS LE_Events::destroyInstance()

using Callback = std::function<void(void*)>;

class LE_EventBus
{
    private:
        std::map<std::string, Callback> listeners;

    public:
        LE_EventBus ();

        ~LE_EventBus ();

        void subscribe(Callback cb, std::string listenerId);

        void unsubscribe(std::string listenerId);

        void emit(void* eventData);
};

class LE_Events
{
    private:
        /*
         * @brief Stores the LE_Events instace
         * */
        static LE_Events* the_instance;

        std::map<std::string, LE_EventBus*> eventBuses;

        LE_Events ();
    public:

        ~LE_Events ();

        LE_Events* Instance ( void );

        void destroyInstance ();

        LE_EventBus* registerEventBus (std::string busId);

        void uregisterEventBus (std::string busId);

        LE_EventBus* getEventBus (std::string busId);

        /**
         * @brief creates an event bus and create a callback subscription
         * */
        void registerCallback (std::string busId, Callback cb, std::string listenerId);

        /**
         * @brief emits an event for a bus Id
         * */
        void emit (std::string busId, void* eventData);

        /**
         * @brief deletes all dynamically allocated buses
         * */
        void clean ();
};


#endif

