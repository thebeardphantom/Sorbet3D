#include "../pch.h"
#include "mesh_gpu.h"
#include "mesh_cpu.h"

namespace objects
{
	mesh_gpu::mesh_gpu(mesh_cpu& mesh_cpu)
	{
		const std::vector<glm::vec3> verts = mesh_cpu.get_verts();
		const std::vector<glm::vec3> colors = mesh_cpu.get_colors();
		const size_t vert_count = verts.size();
		const bool has_colors = !colors.empty();

		if (has_colors)
		{
			for (size_t i = 0; i < vert_count; i++)
			{
				glm::vec3 vert = verts[i];
				vbo_data_.push_back(vert.x);
				vbo_data_.push_back(vert.y);
				vbo_data_.push_back(vert.z);

				glm::vec3 color = colors[i];
				vbo_data_.push_back(color.r);
				vbo_data_.push_back(color.g);
				vbo_data_.push_back(color.b);
			}
		}
		else
		{
			for (size_t i = 0; i < vert_count; i++)
			{
				glm::vec3 vert = verts[i];
				vbo_data_.push_back(vert.x);
				vbo_data_.push_back(vert.y);
				vbo_data_.push_back(vert.z);
			}
		}

		const std::vector<uint32_t> indices = mesh_cpu.get_indices();
		for (uint32_t indice : indices)
		{
			indices_.push_back(indice);
		}

		// Setup vertex array object (VAO)
		glGenVertexArrays(1, &vao_id_);
		glBindVertexArray(vao_id_);

		// Setup vertex buffer object (VBO)
		glGenBuffers(1, &vbo_id_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * vbo_data_.size(),
			vbo_data_.data(),
			GL_STATIC_DRAW);
		const GLsizei stride = has_colors ? 6 * sizeof(GLfloat) : 3 * sizeof(GLfloat);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			stride,
			static_cast<void*>(nullptr));
		if (has_colors)
		{
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				reinterpret_cast<void*>(3 * sizeof(float)));
		}
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(GLuint) * indices_.size(),
			indices_.data(),
			GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	mesh_gpu::~mesh_gpu()
	{
		assert(vao_id_ != 0);
		glDeleteVertexArrays(1, &vao_id_);
		vao_id_ = 0;

		assert(vbo_id_ != 0);
		glDeleteBuffers(1, &vbo_id_);
		vbo_id_ = 0;

		assert(ebo_id_ != 0);
		glDeleteBuffers(1, &ebo_id_);
		ebo_id_ = 0;
	}

	void mesh_gpu::render() const
	{
		glBindVertexArray(vao_id_);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}
