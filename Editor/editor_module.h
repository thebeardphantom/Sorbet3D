#pragma once
#include "../Engine/Modules/engine_module.h"

namespace sorbeditor
{
	class editor_module final : public sorbengine::modules::engine_module
	{
	public:
		SDL_AppResult init() override;
		void collaborate() override;
		void shutdown() override;
		std::string get_name() override;
		event_receive_result receive_event(const SDL_Event& event) override;
	private:
		void on_update();
		void on_render();
	};
}
