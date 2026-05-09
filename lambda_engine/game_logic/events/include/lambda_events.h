#ifndef _LAMBDA_EVENTS_H_
#define _LAMBDA_EVENTS_H_

#include <map>
#include <string>
#include <functional>

/**
 * @brief Global accessor macro for the LE_Events singleton.
 */
#define LE_EVENTS LE_Events::Instance()

/**
 * @brief Destroys the global LE_Events singleton instance.
 */
#define QUIT_LE_EVENTS LE_Events::destroyInstance()

/**
 * @brief Generic callback type used by event listeners.
 *
 * The callback receives a raw pointer to event data.
 * The listener is responsible for casting the pointer
 * to the correct event type.
 */
using Callback = std::function<void(void*)>;

/**
 * @brief Represents a single event bus.
 *
 * An event bus manages a set of listeners identified
 * by a listener ID. When an event is emitted, all
 * subscribed listeners are notified.
 */
class LE_EventBus
{
    private:

        /**
         * @brief Stores all listeners subscribed to this bus.
         *
         * Key   -> listener ID
         * Value -> callback function
         */
        std::map<std::string, Callback> listeners;

    public:

        /**
         * @brief Constructs an empty event bus.
         */
        LE_EventBus ();

        /**
         * @brief Destroys the event bus.
         */
        ~LE_EventBus ();

        /**
         * @brief Subscribes a listener callback to this bus.
         *
         * @param cb Callback function to execute when the
         *           event is emitted.
         *
         * @param listenerId Unique identifier for the listener.
         */
        void subscribe(Callback cb, std::string listenerId);

        /**
         * @brief Removes a listener from this bus.
         *
         * @param listenerId ID of the listener to remove.
         */
        void unsubscribe(std::string listenerId);

        /**
         * @brief Emits an event to all subscribed listeners.
         *
         * @param eventData Pointer to the event payload.
         */
        void emit(void* eventData);
};

/**
 * @brief Singleton manager for all event buses.
 *
 * This class stores and manages multiple named
 * event buses that can be used throughout the engine.
 */
class LE_Events
{
    private:

        /**
         * @brief Stores the singleton instance.
         */
        static LE_Events* the_instance;

        /**
         * @brief Stores all registered event buses.
         *
         * Key   -> bus ID
         * Value -> event bus pointer
         */
        std::map<std::string, LE_EventBus*> eventBuses;

        /**
         * @brief Private constructor for singleton pattern.
         */
        LE_Events ();

    public:

        /**
         * @brief Destroys the event manager.
         */
        ~LE_Events ();

        /**
         * @brief Returns the singleton instance.
         *
         * Creates the instance if it does not exist.
         *
         * @return Pointer to the LE_Events singleton.
         */
        static LE_Events* Instance ( void );

        /**
         * @brief Destroys the singleton instance.
         */
        static void destroyInstance ();

        /**
         * @brief Creates and registers a new event bus.
         *
         * @param busId Unique identifier for the bus.
         *
         * @return Pointer to the created event bus.
         */
        LE_EventBus* registerEventBus (std::string busId);

        /**
         * @brief Unregisters and deletes an event bus.
         *
         * @param busId ID of the bus to remove.
         */
        void uregisterEventBus (std::string busId);

        /**
         * @brief Retrieves an existing event bus.
         *
         * @param busId ID of the requested bus.
         *
         * @return Pointer to the event bus or nullptr
         *         if the bus does not exist.
         */
        LE_EventBus* getEventBus (std::string busId);

        /**
         * @brief Registers a callback into a bus.
         *
         * Creates the bus if it does not already exist.
         *
         * @param busId ID of the target event bus.
         * @param cb Callback function to subscribe.
         * @param listenerId Unique identifier for the listener.
         */
        void registerCallback (
            std::string busId,
            Callback cb,
            std::string listenerId
        );

        /**
         * @brief unregister all callbacks from listener id
         * */
        void dropListener (
            std::string listenerId
        );

        /**
         * @brief Emits an event to a specific bus.
         *
         * @param busId ID of the target bus.
         * @param eventData Pointer to the event payload.
         */
        void emit (std::string busId, void* eventData);

        /**
         * @brief Deletes all dynamically allocated buses.
         */
        void clean ();
};

#endif
