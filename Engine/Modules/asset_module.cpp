#include "../pch.h"
#include "asset_module.h"
#include <SDL3/SDL_filesystem.h>

SDL_AppResult AssetModule::Init()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Base path: %s", SDL_GetBasePath());
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Current directory: %s", SDL_GetCurrentDirectory());
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Pref path: %s", SDL_GetPrefPath("post.ghost", "sorbet3D"));
	return SDL_APP_CONTINUE;
}

void AssetModule::Cleanup()
{

}

const aiScene* AssetModule::GetScene()
{
	return importer.GetScene();
}

std::string AssetModule::GetName()
{
	return "AssetModule";
}

ENGINE_API const aiScene* AssetModule::LoadModel(std::string path)
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
	const auto scene = importer.ReadFile(path, flags);
	if (scene == nullptr)
	{
		throw std::runtime_error(std::format("Failed to load model from path: %s", path));
		//SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Failed to load model from path: %s", path.c_str());  
		return nullptr;
	}

	return scene;
}
