#pragma once
#include <glm/matrix.hpp>
#include "Objects/mesh_cpu.h"

namespace sorbengine
{
	struct render_command
	{
		std::weak_ptr<objects::mesh_cpu> mesh;

		glm::mat4 model_matrix = glm::mat4(1.0f);

		explicit render_command(const std::shared_ptr<objects::mesh_cpu>& mesh_shared_ptr)
		{
			mesh = mesh_shared_ptr;
		}
	};
}
