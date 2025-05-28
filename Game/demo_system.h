#pragma once
#include "../Engine/entity_system.h"
#include "../Engine/Objects/mesh_cpu.h"

class demo_system final : public entity_system
{
public:
	~demo_system() override = default;

	void tick(tick_args& args) override;
	void init() override;
	std::string get_name() override;

private:
	// Fields
	std::shared_ptr<objects::mesh_cpu> mesh_;
};
