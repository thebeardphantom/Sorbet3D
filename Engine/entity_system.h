#pragma once
#include "pch.h"

class entity_system
{
public:
	virtual ~entity_system() = default;

	struct tick_args
	{
		double_t delta_time;
		entt::registry& registry;
	};

	virtual void init() = 0;
	virtual void tick(tick_args& args) = 0;
	virtual std::string get_name() = 0;
};
