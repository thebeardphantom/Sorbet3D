#pragma once
#include <glm/ext/quaternion_float.hpp>

#include "transform.h"

namespace objects
{
	class camera
	{
	public:
		transform& get_transform();

	private:
		transform transform_;
	};
}
