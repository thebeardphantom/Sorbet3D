#pragma once
#include <assimp/mesh.h>
#include <glm/vec3.hpp>
#include "../engine_api.h"

namespace sorbet::objects
{
	class mesh_gpu;

	class mesh_cpu
	{
	public:
		// Constructors / Destructors
		ENGINE_API explicit mesh_cpu(const aiMesh* ai_mesh);

		ENGINE_API ~mesh_cpu();

		mesh_gpu& get_mesh_gpu();

		std::vector<glm::vec3>& get_verts();
		std::vector<glm::vec3>& get_normals();
		std::vector<uint32_t>& get_indices();
		std::vector<glm::vec3>& get_colors();

	private:
		// Fields
		std::vector<glm::vec3> verts_;
		std::vector<glm::vec3> normals_;
		std::vector<uint32_t> indices_;
		std::vector<glm::vec3> colors_;
		std::unique_ptr<mesh_gpu> mesh_gpu_;
	};
}
