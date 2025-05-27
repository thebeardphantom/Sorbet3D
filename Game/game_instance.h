#pragma once

class game_instance
{
public:
	static game_instance& get_instance();

	void initialize();

	void quit() const;

private:
	int tick_callback_id_ = 0;

	int quit_callback_id_ = 0;
};
