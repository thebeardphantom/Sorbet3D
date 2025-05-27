#pragma once
#include "engine_module.h"
#include <SDL3/SDL_init.h>
#include <assimp/mesh.h>
#include "../Objects/render_mesh.h"

class RenderModule : public EngineModule
{
public:
	ENGINE_API SDL_AppResult Init() override;
	ENGINE_API void Cleanup() override;
	ENGINE_API std::string GetName() override;

public:
	// Methods
	ENGINE_API void Submit(std::shared_ptr<RenderMesh> renderMesh);
	void Render();
	bool WireframeMode;

private:
	// Methods
	SDL_AppResult InitSDLWindow();
	SDL_AppResult InitSDLOpenGL();
	SDL_AppResult InitRenderStatics();
	void PreRender();
	void RenderInternal();
	void PostRender();
	GLuint CreateShader(GLenum shaderType, const GLchar* const* string);
	
	template <typename... T>
	GLuint CreateShaderProgram(T... shaders)
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating shader program.");
		GLuint shaderProgram = glCreateProgram();
		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created shader program %d.", shaderProgram);
		(glAttachShader(shaderProgram, shaders), ...);

		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Linking shader program %d.", shaderProgram);
		glLinkProgram(shaderProgram);

		int success;
		char info[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (success)
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Activating shader program %d.", shaderProgram);
			glUseProgram(shaderProgram);

			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Deleting attached shaders.");
			(glDeleteShader(shaders), ...);
		}
		else
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader program %d link error:\n%s\n", shaderProgram, info);
		}
		return shaderProgram;
	}

private:
	// Fields
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_GLContext glContext = nullptr;
	GLuint shaderProgram;
	std::vector<std::shared_ptr<RenderMesh>> renderList;
};

