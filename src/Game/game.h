#pragma once
#include "game_instance.h"

namespace demo_game
{
	class game
	{
	public:
		static void init();

	private:
		static game_instance& get_instance();
	};

}