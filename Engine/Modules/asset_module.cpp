#include "../pch.h"
#include "asset_module.h"
#include <SDL3/SDL_filesystem.h>


namespace modules
{
	SDL_AppResult asset_module::init()
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Base path: %s", SDL_GetBasePath());
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Current directory: %s", SDL_GetCurrentDirectory());
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Pref path: %s", SDL_GetPrefPath("post.ghost", "sorbet3D"));
		return SDL_APP_CONTINUE;
	}

	void asset_module::cleanup() {}

	const aiScene* asset_module::get_scene() const
	{
		return importer_.GetScene();
	}

	std::string asset_module::get_name()
	{
		return "asset_module";
	}

	ENGINE_API const aiScene* asset_module::load_model(std::string path)
	{
		static uint32_t flags =
			aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace
			| aiProcess_JoinIdenticalVertices
			| aiProcess_OptimizeMeshes
			| aiProcess_ImproveCacheLocality
			| aiProcess_SortByPType;

		path = SDL_GetBasePath() + path;
		SDL_LogVerbose(SDL_LOG_CATEGORY_CUSTOM, "Loading model from path: %s", path.c_str());
		const auto scene = importer_.ReadFile(path, flags);
		if (scene == nullptr)
		{
			throw std::runtime_error(std::format("Failed to load model from path: %s", path));
		}

		return scene;
	}
}
