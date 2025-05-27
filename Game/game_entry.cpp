#include "pch.h"
#include "game_instance.h"

extern "C" __declspec(dllexport) void game_entry_point()
{
	game_instance::get_instance().initialize();
}
