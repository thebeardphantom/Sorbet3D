#pragma once
#include <sorbengine/engine.h>
#include <sorbengine/engine_api.h>
#include "editor_instance.h"
#include "editor_module.h"

namespace sorbeditor
{
	class editor
	{
	public:
		static void init();

		template <typename T>
		static T& create_window()
		{
			return sorbengine::engine::get_module<editor_module>()
				.create_window<T>();
		}

	private:
		ENGINE_API static editor_instance& get_instance();
	};
}
