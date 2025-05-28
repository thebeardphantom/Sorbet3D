#include "../pch.h"
#include "asset_module.h"
#include <filesystem>
#include <assimp/Importer.hpp>
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
		auto vert_src = load_shader_stage(path, "vert");
		auto frag_src = load_shader_stage(path, "frag");
		return std::make_unique<objects::shader>(vert_src, frag_src);
	}

	std::string asset_module::load_shader_stage(const std::string& path, const std::string& ext)
	{
		size_t data_size;
		std::string stage_path = SDL_GetBasePath() + path + "." + ext;
		void* data = SDL_LoadFile(stage_path.c_str(), &data_size);

		if (data == nullptr)
		{
			const std::string msg = std::format("Failed to load {} shader from path: {}", ext, stage_path);
			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, msg.c_str());
			throw std::runtime_error(msg);
		}

		auto src = std::string(static_cast<char*>(data), data_size);
		SDL_free(data);
		return src;
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
		size_t data_size;
		void* data = SDL_LoadFile(path.c_str(), &data_size);

		const auto ext = get_file_ext(path);
		const auto scene = importer.ReadFileFromMemory(data, data_size, flags, ext.c_str());
		SDL_free(data);
		if (scene == nullptr)
		{
			const std::string msg = std::format(
				"Failed to load model from path '{}' with error: {}",
				path,
				importer.GetErrorString());
			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, msg.c_str());
			throw std::runtime_error(msg);
		}

		return std::make_unique<objects::mesh_cpu>(scene->mMeshes[0]);
	}

	std::string asset_module::get_file_ext(const std::string& path)
	{
		const std::filesystem::path fs_path = path;
		return fs_path.extension().string();
	}

	std::string asset_module::get_name()
	{
		return "asset_module";
	}
}
