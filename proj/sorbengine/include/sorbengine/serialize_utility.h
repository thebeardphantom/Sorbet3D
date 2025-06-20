#pragma once
#include <cereal/archives/json.hpp>
#include <string>

namespace sorbengine::utility::serialization
{
	template <class Archive>
	void begin_array(Archive& archive, const char* name);

	template <class Archive>
	void begin_element(Archive& archive);

	template <class Archive>
	void end_element(Archive& archive);

	template <class Archive>
	void end_array(Archive& archive);
	
	template<class Archive>
	void begin_array(Archive& archive, const char* name)
	{
		if (auto* json = dynamic_cast<cereal::JSONOutputArchive*>(&archive))
		{
			json->setNextName(name);
			json->startNode();
			json->makeArray();
		}
	}

	template<class Archive>
	void begin_element(Archive& archive)
	{
		if (auto* json = dynamic_cast<cereal::JSONOutputArchive*>(&archive))
		{
			json->startNode();
		}
	}

	template<class Archive>
	void end_element(Archive& archive)
	{
		if (auto* json = dynamic_cast<cereal::JSONOutputArchive*>(&archive))
		{
			json->finishNode();
		}
	}

	template<class Archive>
	void end_array(Archive& archive)
	{
		if (auto* json = dynamic_cast<cereal::JSONOutputArchive*>(&archive))
		{
			json->finishNode();
		}
	}


}