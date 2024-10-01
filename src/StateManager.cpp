#include "StateManager.h"
#include "StartMenu.h"
#include "Gameplay.h"

StateManager::StateManager() {}
StateManager::~StateManager() {}

void StateManager::init() {
	m_running = true;
	changeGameState(GAME_STATE::START_MENU);
}

void StateManager::update() {
	state->update();
	if (currentState == GAME_STATE::START_MENU) {
		if (((StartMenu*)state)->m_gameType != GAME_TYPE::NONE) {
			chosenGameType = ((StartMenu*)state)->m_gameType;
			changeGameState(GAME_STATE::GAMEPLAY);
			return;
		}
		if (((StartMenu*)state)->m_quitClicked) {
			changeGameState(GAME_STATE::NONE);
			return;
		}
	}
	if (currentState == GAME_STATE::GAMEPLAY) {
		if (((Gameplay*)state)->m_winner != PLAYERS::NONE) {
			changeGameState(GAME_STATE::NONE);
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

void StateManager::changeGameState(GAME_STATE newState) {
	currentState = newState;
	if (state != nullptr) {
		state->destroy();
		delete state;
		state = nullptr;
	}
	if (newState == GAME_STATE::START_MENU) {
		state = new StartMenu;
		state->init();
	} else if (newState == GAME_STATE::GAMEPLAY) {
		state = new Gameplay;
		((Gameplay*)state)->gameType = chosenGameType;
		state->init();
	} else if (newState == GAME_STATE::END_SCREEN) {

	} else if (newState == GAME_STATE::NONE) {
		m_running = false;
		destroy();
	}
}