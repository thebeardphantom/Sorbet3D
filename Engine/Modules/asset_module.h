#pragma once
#include "engine_module.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

class AssetModule : public EngineModule
{
public:
	SDL_AppResult Init() override;
	void Cleanup() override;
	std::string GetName() override;

public:
	ENGINE_API const aiScene* LoadModel(std::string path);
	ENGINE_API const aiScene* GetScene();

private:
	Assimp::Importer importer;
};

