#pragma once
#include <memory>
#include <string>
#include <assimp/Importer.hpp>
#include "engine_module.h"
#include "../Objects/mesh_cpu.h"
#include "../Objects/shader.h"

namespace modules
{
	class asset_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;

		void cleanup() override;

		void shutdown() override;

		std::string get_name() override;

		ENGINE_API std::unique_ptr<objects::mesh_cpu> load_model(std::string path);

		ENGINE_API std::unique_ptr<objects::shader> load_shader(const std::string& path);
	};
}
