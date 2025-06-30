#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <sorbengine/path_utility.h>
#include <sorbengine/modules/asset_module.h>
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"

namespace sorbengine::modules
{
	SDL_AppResult asset_module::init()
	{
		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Base path: %s",
			utility::get_base_path().c_str());
		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Current directory: %s",
			utility::get_current_directory().c_str());
		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Pref path: %s",
			utility::get_pref_path("post.ghost", "sorbet3D").c_str());
		return SDL_APP_CONTINUE;
	}

	std::shared_ptr<objects::shader> asset_module::load_shader(const std::string& path)
	{
		const std::string absolute_path = utility::get_absolute_asset_path(path);
		auto vert_src = load_shader_stage(absolute_path, "vert");
		auto frag_src = load_shader_stage(absolute_path, "frag");
		return std::make_shared<objects::shader>(vert_src, frag_src);
	}

	std::shared_ptr<objects::shader> asset_module::load_shader(
		const std::string& vert_path,
		const std::string& frag_path)
	{
		std::string absolute_path = utility::get_absolute_asset_path(vert_path);
		auto vert_src = load_shader_stage(absolute_path, "vert");

		absolute_path = utility::get_absolute_asset_path(frag_path);
		auto frag_src = load_shader_stage(absolute_path, "frag");

		return std::make_shared<objects::shader>(vert_src, frag_src);
	}

	std::string asset_module::load_shader_stage(const std::string& path, const std::string& ext)
	{
		size_t data_size;
		std::string stage_path = std::format("{}.{}", path, ext);
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

	std::shared_ptr<objects::mesh_cpu> asset_module::load_model(const std::string& path)
	{
		static Uint32 flags =
			aiProcess_Triangulate
			| aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace
			| aiProcess_JoinIdenticalVertices
			| aiProcess_OptimizeMeshes
			| aiProcess_ImproveCacheLocality
			| aiProcess_SortByPType;

		const std::string absolute_path = utility::get_absolute_asset_path(path);
		SDL_LogVerbose(SDL_LOG_CATEGORY_CUSTOM, "Loading model from path: %s", absolute_path.c_str());

		Assimp::Importer importer;
		size_t data_size;
		void* data = SDL_LoadFile(absolute_path.c_str(), &data_size);

		const auto ext = utility::get_file_ext(absolute_path);
		const auto scene = importer.ReadFileFromMemory(data, data_size, flags, ext.c_str());
		SDL_free(data);
		if (scene == nullptr)
		{
			const std::string msg = std::format(
				"Failed to load model from path '{}' with error: {}",
				absolute_path,
				importer.GetErrorString());
			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, msg.c_str());
			throw std::runtime_error(msg);
		}

		return std::make_shared<objects::mesh_cpu>(scene->mMeshes[0]);
	}
}
