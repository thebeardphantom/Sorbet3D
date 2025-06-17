#pragma once
#include <format>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <entt/entt.hpp>
#include <SDL3/SDL_init.h>
#include "sorbengine/modules/engine_module.h"

namespace sorbengine
{
	class engine_instance
	{
		struct registered_module
		{
			bool is_external = false;
			bool has_called_init = false;
			std::unique_ptr<modules::engine_module> module;
			std::type_index type_index = typeid(void);
		};

		// Private Fields
		std::vector<registered_module> registered_modules_;
		std::unordered_map<std::type_index, size_t> type_index_to_index_;

		template <class T>
		T& create_module(bool is_external);

		template <class T>
		registered_module& create_module_internal(bool is_external);

		template <class T>
		T& get_module();

		void delete_module(const registered_module& rm);

		// Private Methods
		SDL_AppResult init();
		bool init_module(registered_module& rm, SDL_AppResult& app_result);
		SDL_AppResult receive_event(const SDL_Event& event);
		SDL_AppResult iterate();
		void update() const;
		void render();
		void cleanup_and_shutdown();
		std::unique_ptr<entt::dispatcher> dispatcher_ = nullptr;
		bool is_quitting_ = false;

		friend class engine;
	};

	template <class T>
	T& engine_instance::create_module(const bool is_external)
	{
		auto& module = create_module_internal<T>(is_external).module;
		return static_cast<T&>(*module.get());
	}

	template <class T>
	engine_instance::registered_module& engine_instance::create_module_internal(const bool is_external)
	{
		const std::type_index type = typeid(T);
		if (type_index_to_index_.contains(type))
		{
			throw std::runtime_error("Module type already registered.");
		}

		const auto index = registered_modules_.size();
		registered_modules_.push_back({
			.is_external = is_external,
			.has_called_init = false,
			.module = std::make_unique<T>(),
			.type_index = type
		});

		type_index_to_index_[type] = index;
		return registered_modules_.back();
	}

	template <class T>
	T& engine_instance::get_module()
	{
		const auto type_index = std::type_index(typeid(T));
		const auto index = type_index_to_index_[type_index];
		const auto& module = registered_modules_[index].module;
		if (module == nullptr)
		{
			throw std::runtime_error("Module is nullptr");
		}
		return *static_cast<T*>(module.get());
	}
}
