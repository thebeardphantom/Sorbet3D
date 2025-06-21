#pragma once
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

namespace sorbengine::utility
{
	template <typename Archive>
	constexpr bool is_json_input_archive = std::is_same_v<Archive, cereal::JSONInputArchive>;

	template <typename Archive>
	constexpr bool is_json_output_archive = std::is_same_v<Archive, cereal::JSONOutputArchive>;

	template <typename Archive>
	constexpr bool is_json_archive = is_json_input_archive<Archive> || is_json_output_archive<Archive>;

	template <typename Archive>
	constexpr bool is_xml_input_archive = std::is_same_v<Archive, cereal::XMLInputArchive>;

	template <typename Archive>
	constexpr bool is_xml_output_archive = std::is_same_v<Archive, cereal::XMLOutputArchive>;

	template <typename Archive>
	constexpr bool is_xml_archive = is_xml_input_archive<Archive> || is_xml_output_archive<Archive>;

	template <typename Archive>
	constexpr bool is_text_archive = std::is_base_of_v<cereal::traits::TextArchive, Archive>;

	template <typename Archive>
	constexpr bool is_text_input_archive =
		std::is_base_of_v<cereal::detail::InputArchiveBase, Archive>
		&& is_text_archive<Archive>;

	template <typename Archive>
	constexpr bool is_text_output_archive =
		std::is_base_of_v<cereal::detail::OutputArchiveBase, Archive>
		&& is_text_archive<Archive>;

	template <class Archive>
	void begin_array(Archive& archive, const char* name);

	template <class Archive>
	void begin_element(Archive& archive);

	template <class Archive>
	void end_element(Archive& archive);

	template <class Archive>
	void end_array(Archive& archive);

	template <class Archive>
	void begin_array(Archive& archive, const char* name)
	{
		if constexpr (is_json_output_archive<Archive>)
		{
			archive->setNextName(name);
			archive->startNode();
			archive->makeArray();
		}
		else if constexpr (is_xml_output_archive<Archive>)
		{
			archive->setNextName(name);
			archive->startNode();
		}
	}

	template <class Archive>
	void begin_element(Archive& archive)
	{
		if constexpr (is_json_output_archive<Archive> || is_xml_output_archive<Archive>)
		{
			archive->startNode();
		}
	}

	template <class Archive>
	void end_element(Archive& archive)
	{
		if constexpr (is_json_output_archive<Archive> || is_xml_output_archive<Archive>)
		{
			archive->finishNode();
		}
	}

	template <class Archive>
	void end_array(Archive& archive)
	{
		if constexpr (is_json_output_archive<Archive> || is_xml_output_archive<Archive>)
		{
			archive->finishNode();
		}
	}
}
