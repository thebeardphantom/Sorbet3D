#include "pch.h"
#include "editor.h"

namespace sorbeditor
{
	extern "C" __declspec(dllexport) void editor_entry_point()
	{
		editor::init();
	}

	editor_instance& editor::get_instance()
	{
		static editor_instance instance;
		return instance;
	}

	void editor::init()
	{
		get_instance().init();
	}
}
