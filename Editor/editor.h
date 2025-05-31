#pragma once
#include "editor_instance.h"

namespace sorbeditor
{
	class editor
	{
	public:
		static void init();

	private:
		static editor_instance& get_instance();
	};
}
