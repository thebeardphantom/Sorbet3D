#pragma once
#include "engine_module.h"
#include "../entity_system.h"

namespace modules
{
	class ecs_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;
		void tick();
		ENGINE_API entt::registry& get_registry();

		template <class T>
		T& create_system()
		{
			T* system_ptr = new T();
			const auto generic_system = dynamic_cast<entity_system*>(system_ptr);
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "== Init ECS System %s ==",
				generic_system->get_name().c_str());
			generic_system->init();

			std::unique_ptr<T> system = std::unique_ptr<T>(system_ptr);
			T& ref = *system;
			entity_systems_.push_back(std::move(system));
			return ref;
		}

	private:
		entt::registry registry_;

		std::vector<std::unique_ptr<entity_system>> entity_systems_;
	};
}
