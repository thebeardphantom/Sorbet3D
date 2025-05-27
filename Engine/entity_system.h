#pragma once
#include "pch.h"

class EntitySystem
{
public:
	struct TickArgs
	{
		double_t deltaTime;
		entt::registry& registry;
	};

public:
	virtual void Init() = 0;
	virtual void Tick(TickArgs& args) = 0;
	virtual std::string GetName() = 0;
};

