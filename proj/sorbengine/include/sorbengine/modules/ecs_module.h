#pragma once
#include <typeindex>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/details/helpers.hpp>
#include <entt/entity/registry.hpp>
#include <SDL3/SDL_log.h>
#include "sorbengine/ecs_utility.h"
#include "sorbengine/serializable.h"
#include "sorbengine/serialize_utility.h"
#include "sorbengine/ecs/systems/entity_system.h"
#include "sorbengine/modules/engine_module.h"

namespace sorbengine::modules
{
	class ecs_module final : public engine_module, public serializable
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;
		ENGINE_API entt::registry& get_registry() const;

		template <class T>
		T& create_system();

		template <class Archive>
		void serialize(Archive& archive);

		template <typename Component>
		void register_serializable_type();

	private:
		using serializer_func = std::function<void(
			entt::registry&,
			entt::entity,
			cereal::detail::OutputArchiveBase&)>;
		using deserializer_func = std::function<void(
			entt::registry&,
			entt::entity,
			cereal::detail::InputArchiveBase&)>;
		std::unique_ptr<entt::registry> registry_ = std::make_unique<entt::registry>();
		std::vector<std::unique_ptr<ecs::systems::entity_system>> entity_systems_;
		std::unordered_map<std::type_index, serializer_func> type_to_serialize_func_;
		std::unordered_map<std::type_index, deserializer_func> type_to_deserialize_func_;

		void on_update() const;
	};

	template <class T>
	T& ecs_module::create_system()
	{
		auto system = std::make_unique<T>();
		T& ref = *system;
		const std::string& name = system->get_name();

		SDL_LogVerbose(
			SDL_LOG_CATEGORY_APPLICATION,
			"Created system %s at address %p",
			name.c_str(),
			system.get());
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "== Init ecs System %s ==", name.c_str());

		system->init();
		entity_systems_.push_back(std::move(system));
		return ref;
	}

	template <class Archive>
	void ecs_module::serialize(Archive& archive)
	{
		using archive_base = std::conditional_t<
			std::is_base_of_v<cereal::detail::OutputArchiveBase, Archive>,
			cereal::detail::OutputArchiveBase,
			cereal::detail::InputArchiveBase
		>;

		static_assert(std::is_base_of_v<archive_base, Archive>,
			"Archive must inherit from cereal::{Input,Output}ArchiveBase");

		entt::registry& registry = *registry_;
		auto all_entities = registry.view<entt::entity>();
		if constexpr (std::is_base_of_v<cereal::detail::OutputArchiveBase, archive_base>)
		{
			std::size_t count = 0;
			all_entities.each([&](entt::entity)
			{
				count++;
			});

			utility::serialization::begin_array(archive, "entities");
			all_entities.each([&](entt::entity entity)
			{
				utility::serialization::begin_element(archive);
				// Writing entity as uint32
				archive(cereal::make_nvp("entity_id", utility::ecs::entity_to_uint(entity)));

				utility::serialization::begin_array(archive, "components");
				for (const auto& serializer : type_to_serialize_func_ | std::views::values)
				{
					utility::serialization::begin_element(archive);
					serializer(registry, entity, archive);
					utility::serialization::end_element(archive);
				}
				utility::serialization::end_array(archive);
				utility::serialization::end_element(archive);
			});
			utility::serialization::end_array(archive);
		}
		else
		{
			all_entities.each([&](entt::entity entity)
			{
				for (const auto& deserializer : type_to_deserialize_func_ | std::views::values)
				{
					deserializer(registry, entity, archive);
				}
			});
		}
	}

	template <typename TComponent>
	void ecs_module::register_serializable_type()
	{
		static_assert(std::is_base_of_v<serializable, TComponent>,
			"TComponent must inherit from serializable.");
		type_to_serialize_func_[typeid(TComponent)] = [](
			entt::registry& reg,
			const entt::entity entity,
			cereal::detail::OutputArchiveBase& archive)
			{
				if (!reg.all_of<TComponent>(entity))
				{
					return;
				}

				auto& cmp = reg.get<TComponent>(entity);
				if (auto* json = dynamic_cast<cereal::JSONOutputArchive*>(&archive))
				{
					cmp.serialize(*json);
				}
				else if (auto* binary = dynamic_cast<cereal::BinaryOutputArchive*>(&archive))
				{
					cmp.serialize(*binary);
				}
				else if (auto* portable_binary = dynamic_cast<cereal::PortableBinaryOutputArchive*>(&archive))
				{
					cmp.serialize(*portable_binary);
				}
				else if (auto* xml = dynamic_cast<cereal::XMLOutputArchive*>(&archive))
				{
					cmp.serialize(*xml);
				}
			};

		type_to_deserialize_func_[typeid(TComponent)] = [](
			entt::registry& reg,
			const entt::entity entity,
			cereal::detail::InputArchiveBase& archive)
			{
				auto& cmp = reg.get_or_emplace<TComponent>(entity);
				if (auto* json = dynamic_cast<cereal::JSONInputArchive*>(&archive))
				{
					cmp.serialize(*json);
				}
				else if (auto* binary = dynamic_cast<cereal::BinaryInputArchive*>(&archive))
				{
					cmp.serialize(*binary);
				}
				else if (auto* portable_binary = dynamic_cast<cereal::PortableBinaryInputArchive*>(&archive))
				{
					cmp.serialize(*portable_binary);
				}
				else if (auto* xml = dynamic_cast<cereal::XMLInputArchive*>(&archive))
				{
					cmp.serialize(*xml);
				}
			};
	}
}
