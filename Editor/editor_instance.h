#pragma once
class editor_instance
{
public:
	static editor_instance& get_instance();
	void initialize();
};
