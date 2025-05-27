#pragma once
#include <algorithm>
#include <functional>
#include <vector>

template <typename... Args>
class event
{
public:
	using handler_type = std::function<void(Args...)>;

	// Add handler with operator+=
	int subscribe(handler_type handler)
	{
		handlers_.push_back({++current_id_, handler});
		return current_id_;
	}

	// Remove handler with operator-=
	void unsubscribe(int id)
	{
		handlers_.erase(std::remove_if(handlers_.begin(), handlers_.end(),
				[id](const auto& h)
				{
					return h.id == id;
				}),
			handlers_.end());
	}

	// Fire the event
	void operator()(Args... args) const
	{
		for (const auto& h : handlers_)
		{
			h.handler(args...);
		}
	}

private:
	struct handler_entry
	{
		int id;
		handler_type handler;
	};

	std::vector<handler_entry> handlers_;
	int current_id_ = 0;
};
