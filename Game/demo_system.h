#pragma once
#include "../Engine/entity_system.h"
#include "../Engine/Objects/render_mesh.h"
#include <assimp/scene.h>

class DemoSystem : public EntitySystem
{
public:
	void Tick(TickArgs& deltaTime) override;
	void Init() override;
	std::string GetName() override;

private:
	std::shared_ptr<RenderMesh> renderMesh;
};