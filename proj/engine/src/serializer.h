#pragma once
#include <memory>
namespace sorbengine
{
	class serializer
	{
	public:
		void write();

		std::unique_ptr<std::ostream> output_stream = nullptr;
	};
}