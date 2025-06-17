#pragma once
#include "serializer.h"

namespace sorbengine
{
	class serializable
	{
	public:
		virtual ~serializable() = default;
		virtual void serialize(sorbengine::serializer& serializer) = 0;
		virtual void deserialize(sorbengine::serializer& serializer) = 0;
	};
}