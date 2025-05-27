#include "../pch.h"
#include "render_mesh.h"

namespace objects
{
	void render_mesh::populate_data(const aiMesh* ai_mesh)
	{
		const int vert_count = ai_mesh->mNumVertices;

		for (size_t i = 0; i < vert_count; i++)
		{
			auto vert = ai_mesh->mVertices[i];
			verts_.push_back(vert.x);
			verts_.push_back(vert.y);
			verts_.push_back(vert.z);

			if (ai_mesh->HasVertexColors(0))
			{
				aiColor4D color = ai_mesh->mColors[0][i];
				colors_.push_back(color.r);
				colors_.push_back(color.g);
				colors_.push_back(color.b);
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

		// Setup vertex array object (VAO)
		glGenVertexArrays(1, &vao_id_);
		glBindVertexArray(vao_id_);

		// Setup vertex buffer object (VBO)
		glGenBuffers(1, &vbo_id_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
		glBufferData(GL_ARRAY_BUFFER, verts_.size() * sizeof(GLfloat), verts_.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(nullptr));
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo_id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);


		glGenBuffers(1, &color_buffer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer_id_);
		glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(GLfloat), colors_.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(nullptr));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void render_mesh::render() const
	{
		glBindVertexArray(vao_id_);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void render_mesh::clear_data()
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

		if (color_buffer_id_ != 0)
		{
			glDeleteBuffers(1, &color_buffer_id_);
			color_buffer_id_ = 0;
		}
	}
}
