#include "pch.h"
#include "demo_system.h"
#include "../Engine/engine_instance.h"
#include "../Engine/Modules/asset_module.h"
#include "../Engine/Modules/render_module.h"

void demo_system::tick(tick_args& args)
{
	//SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Ticking demo system with dt %f", args.deltaTime);
	auto& render_module = engine_instance::get_instance().get_engine_module<modules::render_module>();
	render_module.submit(mesh_);
}

void demo_system::init()
{
	modules::asset_module& asset_module = engine_instance::get_instance().get_engine_module<modules::asset_module>();
	std::unique_ptr<objects::mesh_cpu> mesh_cpu = asset_module.load_model("Game/Models/torus.fbx");
	mesh_ = std::move(mesh_cpu);
}

std::string demo_system::get_name()
{
	return "DemoSystem";
}
