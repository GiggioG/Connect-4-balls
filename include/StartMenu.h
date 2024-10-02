#pragma once
#include "State.h"
#include "Gameplay.h"
#include "Button.h"

enum class MENU_LEVEL {
	NONE = 0,
	TOP_LEVEL = 1,
	FRIEND_SUBMENU = 2,
	BOT_SUBMENU = 3
};

class StartMenu : public State {
public:
	StartMenu();
	~StartMenu();

	GAME_TYPE m_gameType = GAME_TYPE::NONE;
	bool m_quitClicked = false;
	MENU_LEVEL menuLevel = MENU_LEVEL::NONE;

	SDL_Texture* backgroundTexture = nullptr;
	SDL_Texture* bannerTexture = nullptr;

	SDL_Texture* playWithFriendTexture = nullptr;
	SDL_Texture* playWithBotTexture = nullptr;
	SDL_Texture* quitTexture = nullptr;
	SDL_Texture* backTexture = nullptr;

	SDL_Texture* friendNormalTexture = nullptr;
	SDL_Texture* friendMadnessTexture = nullptr;

	SDL_Texture* botDumbTexture = nullptr;
	SDL_Texture* botSmartTexture = nullptr;

	bool playWithFriendClicked = false, playWithBotClicked = false;
	bool backClicked = false;
	bool friendNormalClicked = false, friendMadnessClicked = false;
	bool botDumbClicked = false, botSmartClicked = false;

	vector<Button> topLevelButtons;
	vector<Button> friendButtons;
	vector<Button> botButtons;

	void init();
	void update();
	void draw();
	void destroy();
};