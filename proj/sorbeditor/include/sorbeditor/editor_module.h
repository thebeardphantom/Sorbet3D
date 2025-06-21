#pragma once
#include <memory>
#include <vector>
#include <SDL3/SDL_loadso.h>
#include <sorbengine/modules/engine_module.h>
#include "editor_window.h"

namespace sorbeditor
{
	class editor_module final : public sorbengine::modules::engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void cleanup() override;
		void shutdown() override;
		std::string get_name() override;
		event_receive_result receive_event(const SDL_Event& event) override;

		template <typename T>
		T& create_window()
		{
			T* window = new T();
			const auto window_base = static_cast<editor_window*>(window);
			windows_.emplace_back(std::unique_ptr<editor_window>(window_base));
			return *window;
		}

	private:
		std::vector<std::unique_ptr<editor_window>> windows_;
		std::vector<SDL_SharedObject*> shared_objects_;

		void try_load_shared_object(const std::string& name);
		void on_update() const;
		void on_render();
	};
}
