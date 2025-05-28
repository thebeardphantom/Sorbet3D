#include "../pch.h"
#include "mesh_gpu.h"
#include "mesh_cpu.h"

namespace objects
{
	mesh_gpu::mesh_gpu(mesh_cpu& mesh_cpu)
	{
		const std::vector<glm::vec3> verts = mesh_cpu.get_verts();
		const std::vector<glm::vec3> colors = mesh_cpu.get_colors();
		const std::vector<glm::vec3> normals = mesh_cpu.get_normals();


		const size_t vert_count = verts.size();
		const bool has_colors = !colors.empty();
		const bool has_normals = !normals.empty();

		if (has_colors && has_normals)
		{
			for (size_t i = 0; i < vert_count; i++)
			{
				glm::vec3 vert = verts[i];
				vbo_data_.push_back(vert.x);
				vbo_data_.push_back(vert.y);
				vbo_data_.push_back(vert.z);

				glm::vec3 normal = normals[i];
				vbo_data_.push_back(normal.x);
				vbo_data_.push_back(normal.y);
				vbo_data_.push_back(normal.z);

				glm::vec3 color = colors[i];
				vbo_data_.push_back(color.r);
				vbo_data_.push_back(color.g);
				vbo_data_.push_back(color.b);
			}
		}
		else if (has_normals)
		{
			for (size_t i = 0; i < vert_count; i++)
			{
				glm::vec3 vert = verts[i];
				vbo_data_.push_back(vert.x);
				vbo_data_.push_back(vert.y);
				vbo_data_.push_back(vert.z);

				glm::vec3 normal = normals[i];
				vbo_data_.push_back(normal.x);
				vbo_data_.push_back(normal.y);
				vbo_data_.push_back(normal.z);
			}
		}
		else if (has_colors)
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
			vbo_data_.size() * sizeof(GLfloat),
			vbo_data_.data(),
			GL_STATIC_DRAW);

		GLuint attribute_count = 1;
		if (has_colors)
		{
			attribute_count++;
		}
		if (has_normals)
		{
			attribute_count++;
		}

		const GLsizei stride = attribute_count * 3 * sizeof(GLfloat);
		for (GLuint i = 0; i < attribute_count; i++)
		{
			const GLuint offset = i * 3 * sizeof(float);
			glVertexAttribPointer(
				i,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				reinterpret_cast<void*>(offset));
			glEnableVertexAttribArray(i);
		}

		glGenBuffers(1, &ebo_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices_.size() * sizeof(GLuint),
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
