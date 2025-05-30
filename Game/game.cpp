#include "pch.h"
#include "game.h"

#include "game_instance.h"

extern "C" __declspec(dllexport) void game_entry_point()
{
	game::init();
}

void game::init()
{
	get_instance().init();
}

game_instance& game::get_instance()
{
	static game_instance instance;
	return instance;
}
