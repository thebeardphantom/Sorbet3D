#pragma once  
#include "engine_module.h"  
#include <SDL3/SDL_loadso.h>  

class GameLayerModule : public EngineModule  
{  
public:  
    // Methods  
    SDL_AppResult Init() override;  
    void Cleanup() override;  
    std::string GetName() override;  

private:  
    // Fields  
    typedef void (*GameEntryPoint)();  
    SDL_SharedObject* gameModule;
};
