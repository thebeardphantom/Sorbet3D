#pragma once
class editor_window
{
public:
	virtual ~editor_window() = default;
	virtual void draw_imgui() = 0;
};
