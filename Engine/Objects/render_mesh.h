#pragma once
#include <glad/glad.h>
#include <cstdint>
#include <assimp/mesh.h>

class RenderMesh
{
public:
	RenderMesh(aiMesh* mesh)
	{
		PopulateData(mesh);
	}

	~RenderMesh()
	{
		ClearData();
	}

public:
	ENGINE_API void PopulateData(aiMesh* mesh);
	ENGINE_API void ClearData();
	ENGINE_API void Render();

private:
	std::vector<GLfloat> verts;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colors;

	GLuint vboId;
	GLuint vaoId;
	GLuint eboId;
	GLuint colorBufferId;
};