#include "../pch.h"
#include "render_module.h"
#include <glm/gtc/type_ptr.hpp>
#include "time_module.h"
#include "../engine_instance.h"
#include "../enum_strings.h"
#include "../Objects/mesh_cpu.h"

namespace modules
{
	static auto vertex_shader_src = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

	static auto fragment_shader_src = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
} 
)";

	SDL_AppResult render_module::init()
	{
		SDL_AppResult result = init_sdl_window();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}

		result = init_sdl_open_gl();
		if (result != SDL_APP_CONTINUE)
		{
			return result;
		}

		result = init_render_statics();
		return result;
	}


	void render_module::cleanup()
	{
		SDL_GL_DestroyContext(gl_context_);
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
	}

	void render_module::shutdown() {}

	ENGINE_API void render_module::submit(const std::shared_ptr<objects::mesh_cpu>& mesh)
	{
		render_list_.push_back(std::weak_ptr(mesh));
	}

	void render_module::render()
	{
		pre_render();
		render_internal();
		post_render();
	}

	std::string render_module::get_name()
	{
		return "render_module";
	}

	SDL_AppResult render_module::init_sdl_window()
	{
		SDL_Log("== init_sdl_window ==");

		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL_INIT_VIDEO: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		SDL_SetHint(SDL_HINT_VIDEO_FORCE_EGL, "0");
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Creating window.");
		window_ = SDL_CreateWindow("", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (window_ == nullptr)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		return SDL_APP_CONTINUE;
	}

	SDL_AppResult render_module::init_sdl_open_gl()
	{
		SDL_Log(" == init_sdl_open_gl == ");
		gl_context_ = SDL_GL_CreateContext(window_);
		if (gl_context_ == nullptr)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create context: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		// Load OpenGL functions using GLAD (or another loader)
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLAD: %s", SDL_GetError());
			return SDL_APP_FAILURE;
		}

		SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
		return SDL_APP_CONTINUE;
	}

	SDL_AppResult render_module::init_render_statics()
	{
		SDL_Log(" == init_render_statics == ");
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);

		// Create shaders
		const GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, &vertex_shader_src);
		const GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, &fragment_shader_src);
		shader_program_ = create_shader_program(vertex_shader, fragment_shader);

		return SDL_APP_CONTINUE;
	}

	void render_module::pre_render() const
	{
		int width;
		int height;
		SDL_GetWindowSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void render_module::render_internal()
	{
		if (wireframe_mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		const time_module& time_module = engine_instance::get_instance().get_engine_module<modules::time_module>();
		const float time = static_cast<float>(time_module.get_time()) * 2.0f;

		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::degrees(time * 0.01f), glm::vec3(0.0f, 1.0f, 0.0f));

		auto view = glm::mat4(1.0f);

		float offset = (std::sin(time) + 1.0) / 2.0;
		offset = glm::mix(-2.0f, -4.0f, offset);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, offset));

		const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		const glm::mat4 mvp = projection * view * model;
		const int model_loc = glGetUniformLocation(shader_program_, "mvp");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(mvp));

		for (const std::weak_ptr<objects::mesh_cpu>& mesh : render_list_)
		{
			if (const std::shared_ptr<objects::mesh_cpu> mesh_ptr = mesh.lock())
			{
				mesh_ptr->get_mesh_gpu().render();
			}
		}
		render_list_.clear();
	}

	void render_module::post_render() const
	{
		SDL_GL_SwapWindow(window_);
	}

	GLuint render_module::create_shader(const GLenum shader_type, const GLchar* const* src)
	{
		const std::string shader_type_str = enum_strings::to_string(shader_type);
		const char* shader_type_c_str = shader_type_str.c_str();

		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating %s shader.", shader_type_c_str);
		const GLuint shader = glCreateShader(shader_type);
		SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created %s shader %d.", shader_type_c_str, shader);

		glShaderSource(shader, 1, src, nullptr);
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
