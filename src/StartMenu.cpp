#include "StartMenu.h"
#include "Presenter.h"

StartMenu::StartMenu(){}
StartMenu::~StartMenu(){}

void StartMenu::init() {
	backgroundTexture = loadTexture("menu_background.bmp");
	bannerTexture = loadTexture("banner.bmp");

	playWithFriendTexture = loadTexture("playWithFriend.bmp");
	playWithBotTexture = loadTexture("playWithBot.bmp");
	quitTexture = loadTexture("quit.bmp");
	backTexture = loadTexture("back.bmp");

	friendNormalTexture = loadTexture("friendNormal.bmp");
	friendMadnessTexture = loadTexture("friendMadness.bmp");

	botDumbTexture = loadTexture("dumbbot.bmp");
	botSmartTexture = loadTexture("smartbot.bmp");

	menuLevel = MENU_LEVEL::TOP_LEVEL;

	Button btn;

	btn.init(playWithFriendTexture, {200, 200, 256, 128}, &playWithFriendClicked);
	topLevelButtons.push_back(btn);
	btn.init(playWithBotTexture, {200, 500, 256, 128}, &playWithBotClicked);
	topLevelButtons.push_back(btn);
	btn.init(quitTexture, {200, 800, 256, 128}, &m_quitClicked);
	topLevelButtons.push_back(btn);

	btn.init(friendNormalTexture, { 1464, 200, 256, 128 }, &friendNormalClicked);
	friendButtons.push_back(btn);
	btn.init(friendMadnessTexture, { 1464, 500, 256, 128 }, &friendMadnessClicked);
	friendButtons.push_back(btn);
	btn.init(backTexture, { 1464, 800, 256, 128 }, &backClicked);
	friendButtons.push_back(btn);
	botButtons.push_back(btn);

	btn.init(botDumbTexture, { 1464, 200, 256, 128 }, &botDumbClicked);
	botButtons.push_back(btn);
	btn.init(botSmartTexture, { 1464, 500, 256, 128 }, &botSmartClicked);
	botButtons.push_back(btn);
}

void StartMenu::update() {
	if (menuLevel == MENU_LEVEL::TOP_LEVEL) {
		for (Button& b : topLevelButtons) { b.update(); }

		if (playWithFriendClicked) { menuLevel = MENU_LEVEL::FRIEND_SUBMENU; }
		if (playWithBotClicked) { menuLevel = MENU_LEVEL::BOT_SUBMENU; }
	}
	if (menuLevel == MENU_LEVEL::FRIEND_SUBMENU) {
		for (Button& b : friendButtons) { b.update(); }

		if (friendNormalClicked) { m_gameType = GAME_TYPE::NORMAL_MULTIPLAYER; }
		if (friendMadnessClicked) { m_gameType = GAME_TYPE::MADNESS_MULTIPLAYER; }
		if (backClicked) { menuLevel = MENU_LEVEL::TOP_LEVEL; }
	}
	if (menuLevel == MENU_LEVEL::BOT_SUBMENU) {
		for (Button& b : botButtons) { b.update(); }

		if (botDumbClicked) { m_gameType = GAME_TYPE::NORMAL_WEAK_BOT; }
		if (botSmartClicked) { m_gameType = GAME_TYPE::NORMAL_STRONG_BOT; }
		if (backClicked) { menuLevel = MENU_LEVEL::TOP_LEVEL; }
	}
}

void StartMenu::draw() {
	drawObject(backgroundTexture);
	drawObject(bannerTexture, { 560, 200, 800, 305});
	if (menuLevel == MENU_LEVEL::TOP_LEVEL) {
		for (const Button& b : topLevelButtons) { b.draw(); }
	}
	if (menuLevel == MENU_LEVEL::FRIEND_SUBMENU) {
		for (const Button& b : friendButtons) { b.draw(); }
	}
	if (menuLevel == MENU_LEVEL::BOT_SUBMENU) {
		for (const Button& b : botButtons) { b.draw(); }
	}
}

void StartMenu::destroy() {
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyTexture(bannerTexture);

	SDL_DestroyTexture(playWithBotTexture);
	SDL_DestroyTexture(playWithFriendTexture);
	SDL_DestroyTexture(quitTexture);
	SDL_DestroyTexture(backTexture);

	SDL_DestroyTexture(friendNormalTexture);
	SDL_DestroyTexture(friendMadnessTexture);

	SDL_DestroyTexture(botDumbTexture);
	SDL_DestroyTexture(botSmartTexture);
}