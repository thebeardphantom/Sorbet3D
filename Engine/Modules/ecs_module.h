#pragma once
#include "engine_module.h"
#include "../ecs/systems/entity_system.h"

namespace sorbengine::modules
{
	class ecs_module final : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;
		void tick() const;
		ENGINE_API entt::registry& get_registry() const;

		template <class T>
		T& create_system()
		{
			T* system_ptr = new T();

			const auto generic_system_ptr = static_cast<ecs::systems::entity_system*>(system_ptr);
			const std::string& name = generic_system_ptr->get_name();
			SDL_LogVerbose(
				SDL_LOG_CATEGORY_APPLICATION,
				"Created system %s at address %p",
				name.c_str(),
				system_ptr);
			SDL_LogDebug(
				SDL_LOG_CATEGORY_APPLICATION,
				"== Init ECS System %s ==",
				name.c_str());
			generic_system_ptr->init();

			entity_systems_.push_back(std::unique_ptr<ecs::systems::entity_system>(generic_system_ptr));
			return *system_ptr;
		}

	private:
		std::unique_ptr<entt::registry> registry_ = std::make_unique<entt::registry>();

		std::vector<std::unique_ptr<ecs::systems::entity_system>> entity_systems_;
	};
}
