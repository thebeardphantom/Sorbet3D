#pragma once
#include <entt/entity/registry.hpp>
#include <SDL3/SDL_log.h>
#include "engine_module.h"
#include "sorbengine/ecs/systems/entity_system.h"

namespace sorbengine::modules
{
	class ecs_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;
		ENGINE_API entt::registry& get_registry() const;

		template <class T>
		T& create_system();

	private:
		std::unique_ptr<entt::registry> registry_ = std::make_unique<entt::registry>();
		std::vector<std::unique_ptr<ecs::systems::entity_system>> entity_systems_;

		void on_update() const;
	};

	template <class T>
	T& ecs_module::create_system()
	{
		auto system = std::make_unique<T>();
		T& ref = *system;
		const std::string& name = system->get_name();

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Created system %s at address %p",
			name.c_str(),
			system.get());
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "== Init ECS System %s ==", name.c_str());

		system->init();
		entity_systems_.push_back(std::move(system));
		return ref;
	}
}
