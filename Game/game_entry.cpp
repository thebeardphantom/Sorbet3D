#include "pch.h"
#include "../Engine/engine_instance.h"
#include "game_instance.h"

extern "C" __declspec(dllexport) void GameEntryPoint() {
	GameInstance::GetInstance().Initialize();
}