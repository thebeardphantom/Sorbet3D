#include "../pch.h"
#include "shader.h"
#include "../enum_strings.h"

namespace objects
{
	shader::shader(const std::string& vertex_src, const std::string& frag_src)
	{
		const GLuint vert_stage = create_shader_stage(GL_VERTEX_SHADER, vertex_src);
		const GLuint frag_stage = create_shader_stage(GL_FRAGMENT_SHADER, frag_src);
		create_shader_program(vert_stage, frag_stage);
	}

	void shader::use() const
	{
		glUseProgram(shader_program_);
	}

	GLuint shader::get_id() const
	{
		return shader_program_;
	}

	GLuint shader::create_shader_stage(const GLenum shader_type, const std::string& src)
	{
		const std::string shader_type_str = enum_strings::to_string(shader_type);
		const char* shader_type_c_str = shader_type_str.c_str();

		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating %s shader.", shader_type_c_str);
		const GLuint shader = glCreateShader(shader_type);
		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created %s shader %d.", shader_type_c_str, shader);

		const char* src_c_str = src.c_str();
		glShaderSource(shader, 1, &src_c_str, nullptr);
		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Compiling %s shader %d.", shader_type_c_str, shader);
		glCompileShader(shader);

		int success;
		char info[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s shader %d compilation error:%s", shader_type_c_str, shader, info);
		}
		return shader;
	}
}
