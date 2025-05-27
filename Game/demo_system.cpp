#include "pch.h"
#include "../Engine/engine_instance.h"
#include "../Engine/Modules/asset_module.h"
#include "../Engine/Modules/render_module.h"
#include "demo_system.h"
#include <entt/entt.hpp>
#include <iostream>
#include <SDL3/SDL_log.h>

void DemoSystem::Tick(TickArgs& args)
{
	//SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Ticking demo system with dt %f", args.deltaTime);
	auto& renderModule = EngineInstance::GetInstance().GetEngineModule<RenderModule>();
	renderModule.Submit(renderMesh);
}

void DemoSystem::Init()
{
	AssetModule& assetModule = EngineInstance::GetInstance().GetEngineModule<AssetModule>();
	auto scene = assetModule.LoadModel("monkey.fbx");
	renderMesh = std::shared_ptr<RenderMesh>(new RenderMesh(scene->mMeshes[0]));
}

std::string DemoSystem::GetName()
{
	return "DemoSystem";
}
