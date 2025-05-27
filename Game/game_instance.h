#pragma once

class GameInstance
{
public:
	static GameInstance& GetInstance();

	void Initialize();

	void Quit() const;

private:
	int tickCallbackId = 0;

	int quitCallbackId = 0;
};

