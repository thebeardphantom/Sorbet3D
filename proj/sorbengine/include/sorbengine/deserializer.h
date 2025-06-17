#pragma once
#include <memory>

namespace sorbengine
{
	class deserializer
	{
	public:
		void read();

		std::unique_ptr<std::istream> input_stream = nullptr;
	};
}
