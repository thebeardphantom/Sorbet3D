#pragma once
#include "../Engine/ECS/Systems/entity_system.h"

class spin_system final : public ecs::systems::entity_system
{
public:
	void init() override;
	void tick(tick_args& args) override;
	std::string get_name() override;
};
