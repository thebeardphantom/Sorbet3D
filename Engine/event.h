#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

template <typename... Args>
class Event {
public:
    using HandlerType = std::function<void(Args...)>;

    // Add handler with operator+=
    int Subscribe(HandlerType handler)
    {
        handlers.push_back({ ++currentId, handler });
        return currentId;
    }

    // Remove handler with operator-=
    void Unsubscribe(int id)
    {
        handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
            [id](const auto& h) { return h.id == id; }),
            handlers.end());
    }

    // Fire the event
    void operator()(Args... args) const
    {
        for (const auto& h : handlers)
        {
            h.handler(args...);
        }
    }

private:
    struct HandlerEntry {
        int id;
        HandlerType handler;
    };

    std::vector<HandlerEntry> handlers;
    int currentId = 0;
};
