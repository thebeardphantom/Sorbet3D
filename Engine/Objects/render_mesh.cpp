#include "../pch.h"
#include "render_mesh.h"

void RenderMesh::PopulateData(aiMesh* aiMesh)
{
	const int pCount = aiMesh->mNumVertices;

	for (size_t i = 0; i < pCount; i++)
	{
		auto vert = aiMesh->mVertices[i];
		verts.push_back(vert.x);
		verts.push_back(vert.y);
		verts.push_back(vert.z);

		if (aiMesh->HasVertexColors(0))
		{
			aiColor4D color = aiMesh->mColors[0][i];
			colors.push_back(color.r);
			colors.push_back(color.g);
			colors.push_back(color.b);
		}
	}

	for (size_t i = 0; i < aiMesh->mNumFaces; i++)
	{
		auto& face = aiMesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Setup vertex array object (VAO)
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// Setup vertex buffer object (VBO)
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, &colorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void RenderMesh::Render()
{
	glBindVertexArray(vaoId);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderMesh::ClearData()
{
	assert(vaoId != 0);
	glDeleteVertexArrays(1, &vaoId);
	vaoId = 0;

	assert(vboId != 0);
	glDeleteBuffers(1, &vboId);
	vboId = 0;

	assert(eboId != 0);
	glDeleteBuffers(1, &eboId);
	eboId = 0;

	if (colorBufferId != 0)
	{
		glDeleteBuffers(1, &colorBufferId);
		colorBufferId = 0;
	}
}