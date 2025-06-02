#pragma once
#include "../Engine/Events/engine_events.h"
#include "../Engine/Modules/engine_module.h"

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

	private:
		void on_receive_event(const sorbengine::events::receive_sdlevent_event& event);
		void on_update();
		void on_render();
	};
}
