#include "EndScreen.h"
#include "Presenter.h"

EndScreen::EndScreen(){}
EndScreen::~EndScreen(){}

void EndScreen::init() {
	backgroundTexture = loadTexture("menu_background.bmp");

	playAgainTexture = loadTexture("playAgain.bmp");
	quitTexture = loadTexture("quit.bmp");

	if ((gameType == GAME_TYPE::NORMAL_WEAK_BOT || gameType == GAME_TYPE::NORMAL_STRONG_BOT) && winner == PLAYERS::PLAYER_TWO) {
		winnerBannerTexture = loadTexture("botWon.bmp");
	} else if (winner == PLAYERS::PLAYER_ONE) {
		winnerBannerTexture = loadTexture("player1Won.bmp");
	} else if (winner == PLAYERS::PLAYER_TWO) {
		winnerBannerTexture = loadTexture("player2Won.bmp");
	}

	Button btn;

	btn.init(playAgainTexture, {200, 800, 256, 128}, &m_playAgainClicked);
	buttons.push_back(btn);
	btn.init(quitTexture, { 1464, 800, 256, 128 }, &m_quitClicked);
	buttons.push_back(btn);
}

void EndScreen::update() {
	for (Button& b : buttons) { b.update(); }
}

void EndScreen::draw() {
	drawObject(backgroundTexture);

	drawObject(winnerBannerTexture, {560, 200, 800, 234});

	for (const Button& b : buttons) { b.draw(); }
}

void EndScreen::destroy() {
	SDL_DestroyTexture(backgroundTexture);

	SDL_DestroyTexture(playAgainTexture);
	SDL_DestroyTexture(quitTexture);

	SDL_DestroyTexture(winnerBannerTexture);
}