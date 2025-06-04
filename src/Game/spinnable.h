#pragma once
#include <corecrt_math.h>

namespace demo_game::ecs::components
{
	struct spinnable
	{
		float_t speed = 90.0f;
	};
}
