#pragma once
#include <string>
#include "engine_api.h"

namespace sorbengine::utility
{
	ENGINE_API std::string& replace_all(std::string& str, const std::string& from, const std::string& to);
}
