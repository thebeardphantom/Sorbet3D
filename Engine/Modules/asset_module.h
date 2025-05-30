#pragma once
#include <memory>
#include <string>
#include "engine_module.h"
#include "../Objects/mesh_cpu.h"
#include "../Objects/shader.h"

namespace modules
{
	class asset_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;
		ENGINE_API std::shared_ptr<objects::shader> load_shader(const std::string& path);
		ENGINE_API std::shared_ptr<objects::shader> load_shader(const std::string& vert_path, const std::string& frag_path);
		ENGINE_API std::shared_ptr<objects::mesh_cpu> load_model(std::string path);

	private:
		static std::string get_file_ext(const std::string& path);
		static std::string load_shader_stage(const std::string& path, const std::string& ext);
	};
}
