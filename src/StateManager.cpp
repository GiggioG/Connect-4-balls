#include "StateManager.h"
#include "StartMenu.h"
#include "Gameplay.h"
#include "EndScreen.h"

StateManager::StateManager() {}
StateManager::~StateManager() {}

void StateManager::init() {
	m_running = true;
	changeGameState(GAME_STATE::START_MENU, {});
}

void StateManager::update() {
	state->update();
	if (currentState == GAME_STATE::START_MENU) {
		if (((StartMenu*)state)->m_gameType != GAME_TYPE::NONE) {
			StateTransitionInfo info;
			info.gameplay.gameType = ((StartMenu*)state)->m_gameType;
			changeGameState(GAME_STATE::GAMEPLAY, info);
			return;
		}
		if (((StartMenu*)state)->m_quitClicked) {
			changeGameState(GAME_STATE::NONE, {});
			return;
		}
	}
	if (currentState == GAME_STATE::GAMEPLAY) {
		if (((Gameplay*)state)->m_winner != PLAYERS::NONE) {
			StateTransitionInfo info;
			info.endScreen.gameType = ((Gameplay*)state)->gameType;
			info.endScreen.winner = ((Gameplay*)state)->m_winner;
			changeGameState(GAME_STATE::END_SCREEN, info);
			return;
		}
	}
	if (currentState == GAME_STATE::END_SCREEN) {
		if (((EndScreen*)state)->m_playAgainClicked) {
			changeGameState(GAME_STATE::START_MENU, {});
			return;
		}
		if (((EndScreen*)state)->m_quitClicked) {
			changeGameState(GAME_STATE::NONE, {});
			return;
		}
	}
}

void StateManager::draw() {
	if (state != nullptr) {
		state->draw();
	}
}

void StateManager::destroy() {
	m_running = false;
	if (state != nullptr) {
		state->destroy();
		delete state;
	}
	state = nullptr;
}



void StateManager::changeGameState(GAME_STATE newState, StateTransitionInfo info) {
	currentState = newState;
	if (state != nullptr) {
		state->destroy();
		delete state;
		state = nullptr;
	}
	if (newState == GAME_STATE::START_MENU) {
		state = new StartMenu;
	} else if (newState == GAME_STATE::GAMEPLAY) {
		state = new Gameplay;
		((Gameplay*)state)->gameType = info.gameplay.gameType;
	} else if (newState == GAME_STATE::END_SCREEN) {
		state = new EndScreen;
		((EndScreen*)state)->gameType = info.endScreen.gameType;
		((EndScreen*)state)->winner = info.endScreen.winner;
	} else if (newState == GAME_STATE::NONE) {
		m_running = false;
		destroy();
		return;
	}
	state->init();
}