#pragma once
#include <glad/glad.h>
#include "../engine_api.h"

namespace objects
{
	class mesh_cpu;

	class mesh_gpu
	{
	public:
		ENGINE_API explicit mesh_gpu(mesh_cpu& mesh_cpu);
		ENGINE_API ~mesh_gpu();
		void render() const;

	private:
		// Fields
		std::vector<GLfloat> vbo_data_;
		std::vector<GLuint> indices_;

		GLuint vbo_id_;
		GLuint vao_id_;
		GLuint ebo_id_;
	};
}
