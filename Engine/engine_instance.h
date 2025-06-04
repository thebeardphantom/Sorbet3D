#pragma once
#include <format>
#include <memory>
#include <typeindex>
#include <vector>
#include <entt/entt.hpp>
#include <SDL3/SDL_init.h>
#include "Modules/engine_module.h"

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

		template <class T>
		T& create_module(const bool is_external)
		{
			auto& module = create_module_internal<T>(is_external).module;
			return static_cast<T&>(*module.get());
		}

		template <class T>
		registered_module& create_module_internal(const bool is_external)
		{
			const std::type_index type = typeid(T);
			if (type_index_to_registered_module_.contains(type))
			{
				throw std::runtime_error("Module type already registered.");
			}

			std::unique_ptr<T> module_ptr = std::make_unique<T>();
			auto rm_ptr = new registered_module{
				.is_external = is_external,
				.has_called_init = false,
				.module = std::move(module_ptr),
				.type_index = type
			};

			auto rm = std::shared_ptr<registered_module>(rm_ptr);
			registered_modules_.emplace_back(rm);

			type_index_to_registered_module_[type] = std::weak_ptr(rm);

			std::ranges::stable_sort(
				registered_modules_,
				[](const std::shared_ptr<registered_module>& a, const std::shared_ptr<registered_module>& b)
				{
					return a->module->get_priority() < b->module->get_priority();
				});

			return *rm;
		}

		template <class T>
		T& get_module()
		{
			const auto type_index = std::type_index(typeid(T));
			const auto registered_module = type_index_to_registered_module_[type_index];
			const auto& module = registered_module.lock()->module;
			if (module == nullptr)
			{
				throw std::runtime_error("Module is nullptr");
			}
			return *static_cast<T*>(module.get());
		}

		void delete_module(const std::shared_ptr<registered_module>& rm);

		// Private Fields
		std::vector<std::shared_ptr<registered_module>> registered_modules_;
		std::map<std::type_index, std::weak_ptr<registered_module>> type_index_to_registered_module_;

		// Private Methods
		SDL_AppResult init();
		bool init_module(registered_module& rm, SDL_AppResult& app_result);
		SDL_AppResult receive_event(const SDL_Event& event);
		SDL_AppResult iterate();
		void update() const;
		void render();
		void cleanup_and_shutdown();
		void cleanup_and_shutdown_modules(bool external_modules);
		std::unique_ptr<entt::dispatcher> dispatcher_ = nullptr;
		bool is_quitting_ = false;

		friend class engine;
	};
}
