#include "pch.h"
#include "editor_instance.h"

extern "C" __declspec(dllexport) void editor_entry_point()
{
	editor_instance::get_instance().initialize();
}
