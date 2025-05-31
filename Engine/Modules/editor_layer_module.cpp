#include "../pch.h"
#include "editor_layer_module.h"

namespace sorbet::modules
{
	SDL_AppResult editor_layer_module::init()
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Loading Editor.dll.");
		editor_so_ = SDL_LoadObject("Editor.dll");
		if (editor_so_ == nullptr)
		{
			const char* error = SDL_GetError();
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to load Editor.dll: %s.", error);
			return SDL_APP_CONTINUE;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Locating editor_entry_point.");

		const auto editor_entry_point = SDL_LoadFunction(editor_so_, "editor_entry_point");
		if (editor_entry_point == nullptr)
		{
			const char* error = SDL_GetError();
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Cannot locate editor_entry_point in Editor.dll: %s.", error);
			return SDL_APP_FAILURE;
		}

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Invoking editor_entry_point.");
		editor_entry_point();
		return SDL_APP_CONTINUE;
	}

	void editor_layer_module::collaborate() {}

	void editor_layer_module::cleanup() {}

	void editor_layer_module::shutdown()
	{
		if (editor_so_ != nullptr)
		{
			SDL_UnloadObject(editor_so_);
			editor_so_ = nullptr;
		}
	}

	std::string editor_layer_module::get_name()
	{
		return "editor_layer_module";
	}
}
