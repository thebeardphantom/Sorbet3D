#include "../pch.h"
#include "mesh_cpu.h"
#include "mesh_gpu.h"

namespace objects
{
	mesh_cpu::~mesh_cpu() = default;

	mesh_cpu::mesh_cpu(const aiMesh* ai_mesh)
	{
		const uint32_t vert_count = ai_mesh->mNumVertices;

		for (uint32_t i = 0; i < vert_count; i++)
		{
			const aiVector3D& vert = ai_mesh->mVertices[i];
			verts_.emplace_back(vert.x, vert.y, vert.z);

			if (ai_mesh->HasVertexColors(0))
			{
				const aiColor4D& color = ai_mesh->mColors[0][i];
				colors_.emplace_back(color.r, color.g, color.b);
			}
		}

		for (size_t i = 0; i < ai_mesh->mNumFaces; i++)
		{
			const auto& face = ai_mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices_.push_back(face.mIndices[j]);
			}
		}
	}

	mesh_gpu& mesh_cpu::get_mesh_gpu()
	{
		if (mesh_gpu_ == nullptr)
		{
			mesh_gpu_ = std::make_unique<mesh_gpu>(*this);
		}
		return *mesh_gpu_;
	}

	std::vector<glm::vec3>& mesh_cpu::get_verts()
	{
		return verts_;
	}

	std::vector<uint32_t>& mesh_cpu::get_indices()
	{
		return indices_;
	}

	std::vector<glm::vec3>& mesh_cpu::get_colors()
	{
		return colors_;
	}
}
