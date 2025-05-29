#include "pch.h"
#include "editor_instance.h"

editor_instance& editor_instance::get_instance()
{
	static editor_instance instance;
	return instance;
}

void editor_instance::initialize()
{
	
}
