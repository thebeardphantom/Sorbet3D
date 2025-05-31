#include "pch.h"
#include "editor_instance.h"
#include "editor_module.h"
#include "../Engine/engine.h"

namespace sorbeditor
{
	void editor_instance::init()
	{
		sorbengine::engine::create_module<editor_module>();
	}
}
