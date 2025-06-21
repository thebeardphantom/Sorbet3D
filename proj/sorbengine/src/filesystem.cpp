#include "sorbengine/filesystem.h"
#include "SDL3/SDL_filesystem.h"

namespace sorbengine::utility
{
	bool file_exists(const char* path)
	{
		return SDL_GetPathInfo(path, nullptr);
	}
}
