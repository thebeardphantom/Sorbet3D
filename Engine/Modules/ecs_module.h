#pragma once
#include "engine_module.h"
#include "../entity_system.h"

class ECSModule : public EngineModule
{
public:
	SDL_AppResult Init() override;
	void Cleanup() override;
	std::string GetName() override;
	void Tick();
	ENGINE_API entt::registry& GetRegistry();

	template <class T>
	T& CreateSystem()
	{
		T* systemPtr = new T();
		EntitySystem* genericSystem = dynamic_cast<EntitySystem*>(systemPtr);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "== Init ECS System %s ==", genericSystem->GetName().c_str());
		genericSystem->Init();
		
		std::unique_ptr<T> system = std::unique_ptr<T>(systemPtr);
		T& ref = *system;
		entitySystems.push_back(std::move(system));
		return ref;
	}

private:
	entt::registry registry;

	std::vector<std::unique_ptr<EntitySystem>> entitySystems;
};

