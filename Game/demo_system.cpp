#include "pch.h"
#include "demo_system.h"
#include "../Engine/engine_instance.h"
#include "../Engine/Modules/asset_module.h"
#include "../Engine/Modules/render_module.h"

void demo_system::tick(tick_args& args)
{
	//SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "Ticking demo system with dt %f", args.deltaTime);
	auto& render_module = engine_instance::get_instance().get_engine_module<modules::render_module>();
	render_module.submit(render_mesh_);
}

void demo_system::init()
{
	modules::asset_module& asset_module = engine_instance::get_instance().get_engine_module<modules::asset_module>();
	const auto scene = asset_module.load_model("monkey.fbx");
	render_mesh_ = std::make_shared<objects::render_mesh>(scene->mMeshes[0]);
}

std::string demo_system::get_name()
{
	return "DemoSystem";
}
