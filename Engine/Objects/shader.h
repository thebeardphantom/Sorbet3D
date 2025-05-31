#pragma once
#include <glad/glad.h>
#include "../engine_api.h"

namespace sorbengine::objects
{
	class shader
	{
	public:
		ENGINE_API explicit shader(const std::string& vertex_src, const std::string& frag_src);
		ENGINE_API void use() const;
		ENGINE_API GLuint get_id() const;

	private:
		GLuint shader_program_;

		static GLuint create_shader_stage(GLenum shader_type, const std::string& src);

		template <typename... T>
		void create_shader_program(T... shaders)
		{
			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating shader program.");
			shader_program_ = glCreateProgram();
			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created shader program %d.", shader_program_);
			(glAttachShader(shader_program_, shaders), ...);

			SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Linking shader program %d.", shader_program_);
			glLinkProgram(shader_program_);

			int success;
			char info[512];
			glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);
			if (success)
			{
				SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Activating shader program %d.", shader_program_);
				glUseProgram(shader_program_);

				SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Deleting attached shaders.");
				(glDeleteShader(shaders), ...);
			}
			else
			{
				glGetProgramInfoLog(shader_program_, 512, nullptr, info);
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Shader program %d link error: %s", shader_program_, info);
			}
		}
	};
}
