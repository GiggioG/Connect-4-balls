#pragma once
#include "State.h"
#include "Gameplay.h"

union StateTransitionInfo {
	struct {} startMenu;
	struct {
		GAME_TYPE gameType;
	} gameplay;
	struct {
		GAME_TYPE gameType;
		PLAYERS winner;
	} endScreen;
	struct {} none;
};

class StateManager {
public:
	StateManager();
	~StateManager();

	GAME_STATE currentState = GAME_STATE::NONE;
	bool m_running = true;

	void init();
	void update();
	void draw();
	void destroy();
private:
	State* state = nullptr;
	void changeGameState(GAME_STATE newState, StateTransitionInfo info);
};