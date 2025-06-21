#pragma once
#include <glm/matrix.hpp>
#include "smath.h"
#include "objects/mesh_cpu.h"

namespace sorbengine
{
	struct render_command
	{
		std::weak_ptr<objects::mesh_cpu> mesh;

		glm::mat4 model_matrix = utility::identity_matrix;

		explicit render_command(const std::shared_ptr<objects::mesh_cpu>& mesh_shared_ptr)
		{
			mesh = mesh_shared_ptr;
		}
	};
}
