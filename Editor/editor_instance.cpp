#include "pch.h"
#include "editor_instance.h"
#include "editor_module.h"
#include "../Engine/engine.h"

void editor_instance::init()
{
	engine::create_module<editor_module>();
}
