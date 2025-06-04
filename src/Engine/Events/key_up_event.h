#pragma once
#include <SDL3/SDL_keycode.h>

namespace sorbengine::events
{
	struct key_up_event
	{
		SDL_Keycode key;
		SDL_Keymod mod;
	};
}