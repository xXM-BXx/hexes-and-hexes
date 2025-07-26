//
// Created by s453388 on 27.05.2025.
//
#pragma once
#include <functional>
#include <list>
#include <utility>


namespace gl3::engine::events {
    template<typename Owner, typename ...Args>
    class Event {
        friend Owner;

    public:
        using listener_t = typename std::function<void(Args...)>;
        ///A wrapper needed to safely delete Listeners
        struct ListenerWrapper {
            bool enabled = true;
            listener_t callback;
        };
        using container_t = typename std::list<ListenerWrapper>;
        using handle_t = typename container_t::iterator;

        /// Add a listener.
        /// @param listener The callback \ref(std::function) that should be invoked by the event.
        /// @return The iterator to find the listener again later. Needed if e.g. you want to unsubscribe from the event.
        handle_t addListener(listener_t listener) {
            listeners.emplace_back(ListenerWrapper{true, std::move(listener)});
            return std::prev(listeners.end());
        }

        /// Safely removes a listener by their handle.
        /// @param handle An iterator to a listener. Provided on listener creation.
        void removeListener(handle_t handle) {
            handle->enabled = false;
            needsPruning = true;
        }

        /// Remove one listener and add a new one, effectively replacing it.
        /// @param oldHandle An iterator to a listener to delete. Provided on listener creation.
        /// @param newListener The new callback \ref(std::function) that should be invoked by the event.
        /// @return The iterator to find the new listener again later. Needed if e.g. you want to unsubscribe from the event.
        handle_t replaceListener(handle_t oldHandle, listener_t newListener) {
            removeListener(oldHandle);
            return addListener(std::move(newListener));
        }

    private:
        void invoke(Args ...args) {
            for(auto &listener: listeners) {
                if(listener.enabled) {
                    listener.callback(std::forward<Args>(args)...);
                }
            }
            if(needsPruning) pruneDisabled();
        }

        void pruneDisabled() {
            listeners.remove_if([](auto &listener) {
                return !listener.enabled;
            });
            needsPruning = false;
        }

        bool needsPruning = false;
        container_t listeners;
    };
}