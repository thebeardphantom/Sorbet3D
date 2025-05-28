#pragma once
#include "engine_module.h"
#include "../entity_system.h"

namespace modules
{
	class ecs_module final : public engine_module
	{
	public:
		~ecs_module() override = default;
		SDL_AppResult init() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;
		void tick();
		ENGINE_API entt::registry& get_registry();

		template <class T>
		T& create_system()
		{
			T* system_ptr = new T();

			const auto generic_system_ptr = dynamic_cast<entity_system*>(system_ptr);
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

			entity_systems_.push_back(std::unique_ptr<entity_system>(generic_system_ptr));
			return *system_ptr;
		}

	private:
		entt::registry registry_;

		std::vector<std::unique_ptr<entity_system>> entity_systems_;
	};
}
