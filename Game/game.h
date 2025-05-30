#pragma once
#include "game_instance.h"

class game
{
public:
	static void init();

private:
	static game_instance& get_instance();
};
