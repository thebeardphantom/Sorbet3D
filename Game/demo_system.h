#pragma once
#include "../Engine/entity_system.h"
#include "../Engine/Objects/render_mesh.h"

class demo_system final : public entity_system
{
public:
	// Overridden Methods
	~demo_system() override
	{
		SDL_Log("demo_system dtor called");
	}

	void tick(tick_args& args) override;
	void init() override;
	std::string get_name() override;

private:
	// Fields
	std::shared_ptr<objects::render_mesh> render_mesh_;
};
