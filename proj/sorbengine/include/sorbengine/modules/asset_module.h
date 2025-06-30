#pragma once
#include <memory>
#include <string>
#include "sorbengine/modules/engine_module.h"
#include "sorbengine/objects/mesh_cpu.h"
#include "sorbengine/objects/shader.h"

namespace sorbengine::modules
{
	class asset_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		
		ENGINE_API std::shared_ptr<objects::shader> load_shader(const std::string& path);
		ENGINE_API std::shared_ptr<objects::shader> load_shader(const std::string& vert_path,
			const std::string& frag_path);
		ENGINE_API std::shared_ptr<objects::mesh_cpu> load_model(const std::string& path);

	private:
		static std::string load_shader_stage(const std::string& path, const std::string& ext);
	};
}
