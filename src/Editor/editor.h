#pragma once
#include "editor_instance.h"
#include "editor_module.h"
#include "../Engine/engine.h"
#include "../Engine/engine_api.h"

namespace sorbeditor
{
	class editor
	{
	public:
		static void init();

		template <typename T>
		static T& create_window()
		{
			return sorbengine::engine::get_module<editor_module>().create_window<T>();
		}

	private:
		ENGINE_API static editor_instance& get_instance();
	};
}
