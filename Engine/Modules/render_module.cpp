#include "../pch.h"
#include "../engine_instance.h"
#include "../enum_strings.h"
#include "render_module.h"
#include "time_module.h"
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 pvm;

void main()
{
    gl_Position = pvm * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0f);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
} 
)";

SDL_AppResult RenderModule::Init()
{
	SDL_AppResult result = InitSDLWindow();
	if (result != SDL_APP_CONTINUE)
	{
		return result;
	}

	result = InitSDLOpenGL();
	if (result != SDL_APP_CONTINUE)
	{
		return result;
	}

	result = InitRenderStatics();
	return result;
}


void RenderModule::Cleanup()
{
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

ENGINE_API void RenderModule::Submit(std::shared_ptr<RenderMesh> renderMesh)
{
	renderList.push_back(renderMesh);
}

void RenderModule::Render()
{
	PreRender();
	RenderInternal();
	PostRender();
}

std::string RenderModule::GetName()
{
	return "RenderSystem";
}

SDL_AppResult RenderModule::InitSDLWindow()
{
	SDL_Log("== InitSDL ==");

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
	window = SDL_CreateWindow("", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult RenderModule::InitSDLOpenGL()
{
	SDL_Log(" == InitSDLOpenGL == ");
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create context: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Load OpenGL functions using GLAD (or another loader)
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLAD: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
	return SDL_APP_CONTINUE;
}

SDL_AppResult RenderModule::InitRenderStatics()
{
	SDL_Log(" == InitRenderStatics == ");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Create shaders
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, &vertexShaderSrc);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, &fragmentShaderSrc);
	shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

	return SDL_APP_CONTINUE;
}

void RenderModule::PreRender()
{
	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderModule::RenderInternal()
{
	if (WireframeMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	auto& timeModule = EngineInstance::GetInstance().GetEngineModule<TimeModule>();
	float time = timeModule.GetTime() * 2.0;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::degrees(time * 0.01f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);

	float offset = (std::sin(time) + 1.0) / 2.0;
	offset = glm::mix(-2.0f, -4.0f, offset);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, offset));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	glm::mat4 pvm = projection * view * model;
	int modelLoc = glGetUniformLocation(shaderProgram, "pvm");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pvm));

	for (std::shared_ptr<RenderMesh> mesh : renderList)
	{
		mesh->Render();
	}
	renderList.clear();
}

void RenderModule::PostRender()
{
	SDL_GL_SwapWindow(window);
}

GLuint RenderModule::CreateShader(GLenum shaderType, const GLchar* const* src)
{
	std::string shaderTypeStr = EnumStrings::ToString(shaderType);
	const char* shaderTypeCStr = shaderTypeStr.c_str();

	SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Creating %s shader.", shaderTypeCStr);
	GLuint shader = glCreateShader(shaderType);
	SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Created %s shader %d.", shaderTypeCStr, shader);

	glShaderSource(shader, 1, src, nullptr);
	SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER, "Compiling %s shader %d.", shaderTypeCStr, shader);
	glCompileShader(shader);

	int success;
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s shader %d compilation error:\n%s\n", shaderTypeCStr, shader, info);
	}
	return shader;
}