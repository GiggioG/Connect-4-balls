#pragma once
#include "State.h"
#include "Gameplay.h"
#include "Button.h"

class EndScreen : public State {
public:
	EndScreen();
	~EndScreen();

	bool m_playAgainClicked = false;
	bool m_quitClicked = false;

	GAME_TYPE gameType;
	PLAYERS winner;

	SDL_Texture* backgroundTexture = nullptr;

	SDL_Texture* playAgainTexture = nullptr;
	SDL_Texture* quitTexture = nullptr;

	SDL_Texture* winnerBannerTexture = nullptr;

	vector<Button> buttons;

	void init();
	void update();
	void draw();
	void destroy();
};