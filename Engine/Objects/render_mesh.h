#pragma once
#include "../engine_api.h"
#include <assimp/mesh.h>
#include <glad/glad.h>

namespace objects
{
	class render_mesh
	{
	public:
		// Constructors / Destructors
		explicit render_mesh(const aiMesh* mesh)
		{
			populate_data(mesh);
		}

		~render_mesh()
		{
			clear_data();
		}

		// Public Methods
		ENGINE_API void populate_data(const aiMesh* ai_mesh);
		ENGINE_API void clear_data();
		ENGINE_API void render() const;

	private:
		// Fields
		std::vector<GLfloat> verts_;
		std::vector<GLuint> indices_;
		std::vector<GLfloat> colors_;

		GLuint vbo_id_;
		GLuint vao_id_;
		GLuint ebo_id_;
		GLuint color_buffer_id_;
	};
}
