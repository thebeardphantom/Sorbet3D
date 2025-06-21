#include "sorbeditor/editor_instance.h"
#include <sorbengine/engine.h>
#include "sorbeditor/editor_module.h"

namespace sorbeditor
{
	void editor_instance::init()
	{
		sorbengine::engine::create_module<editor_module>();
	}
}
