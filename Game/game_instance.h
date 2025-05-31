#pragma once

namespace demo_game
{
	class game_instance
	{
		void init();
		void quit() const;
		int tick_callback_id_ = 0;
		int quit_callback_id_ = 0;

		friend class game;
	};
}
