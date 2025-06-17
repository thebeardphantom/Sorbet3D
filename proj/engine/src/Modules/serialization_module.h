#pragma once
#include "engine_module.h"

namespace sorbengine::modules
{
	class serialization_module final : public engine_module
	{
	public:
		std::string get_name() override;
	};
}
