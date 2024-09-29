#pragma once
#include "State.h"
#include "Gameplay.h"

class StateManager {
public:
	StateManager();
	~StateManager();

	GAME_STATE currentState = GAME_STATE::NONE;
	bool m_running = true;
	GAME_TYPE chosenGameType = GAME_TYPE::NONE;

	void init();
	void update();
	void draw();
	void destroy();
private:
	State* state = nullptr;
	void changeGameState(GAME_STATE newState);
};