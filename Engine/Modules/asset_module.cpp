#include "../pch.h"
#include "asset_module.h"
#include <assimp/scene.h>
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

	void asset_module::shutdown() {}

	std::unique_ptr<objects::shader> asset_module::load_shader(const std::string& path)
	{
		const std::string vert_path = SDL_GetBasePath() + path + ".vert";

		size_t data_size;
		void* vert_data = SDL_LoadFile(vert_path.c_str(), &data_size);
		if (vert_data == nullptr)
		{
			return nullptr;
		}
		auto vert_src = std::string(static_cast<char*>(vert_data), data_size);

		const std::string frag_path = SDL_GetBasePath() + path + ".frag";
		void* frag_data = SDL_LoadFile(frag_path.c_str(), &data_size);
		if (frag_data == nullptr)
		{
			return nullptr;
		}
		auto frag_src = std::string(static_cast<char*>(frag_data), data_size);

		//return objects::shader()
		return std::make_unique<objects::shader>(vert_src, frag_src);
	}

	std::string asset_module::get_name()
	{
		return "asset_module";
	}

	ENGINE_API std::unique_ptr<objects::mesh_cpu> asset_module::load_model(std::string path)
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

		Assimp::Importer importer;
		const auto scene = importer.ReadFile(path, flags);
		if (scene == nullptr)
		{
			throw std::runtime_error(std::format("Failed to load model from path: %s", path));
		}

		return std::make_unique<objects::mesh_cpu>(scene->mMeshes[0]);
	}
}
