#include "../pch.h"
#include "asset_module.h"

SDL_AppResult AssetModule::Init()
{
	return SDL_APP_CONTINUE;
}

void AssetModule::Cleanup()
{
    
}

const aiScene* AssetModule::GetScene()
{
    return importer.GetScene();
}

std::string AssetModule::GetName()
{
	return "AssetModule";
}

ENGINE_API const aiScene* AssetModule::LoadModel(std::string path)
{  
    SDL_LogVerbose(SDL_LOG_CATEGORY_CUSTOM, "Loading model from path: %s", path.c_str());  

    static uint32_t flags =
        aiProcess_Triangulate
        | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace
        | aiProcess_JoinIdenticalVertices
        | aiProcess_OptimizeMeshes
        | aiProcess_ImproveCacheLocality
        | aiProcess_SortByPType;
    
    const auto scene = importer.ReadFile(path, flags);
    if (scene == nullptr)  
    {  
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Failed to load model from path: %s", path.c_str());  
        return nullptr;  
    }  
    
    return scene;
}
