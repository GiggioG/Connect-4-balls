#pragma once
#include <vector>
#include <SDL.h>
#include "State.h"
#include "util.h"
#include "Checker.h"
#include "BonusSystem.h"

class BonusSystem; /// forward declaration

enum class PLAYERS {
	NONE = 0,
	PLAYER_ONE = 1,
	PLAYER_TWO = 2,
	TIE = 3
};

enum class CHECKER_TYPES {
	NONE = 0,
	PLAYER_ONE = 1,
	PLAYER_TWO = 2,
	STONE = 3
};

enum class GAME_TYPE {
	NONE = 0,
	NORMAL_MULTIPLAYER = 1,
	NORMAL_WEAK_BOT = 2,
	NORMAL_STRONG_BOT = 3,
	MADNESS_MULTIPLAYER = 4
};

class Gameplay : public State {
public:
	Gameplay();
	~Gameplay();

	PLAYERS m_winner = PLAYERS::NONE;
	GAME_TYPE gameType = GAME_TYPE::NONE;

	void init();
	void update();
	void draw();
	void destroy();
	friend class BonusSystem;
private:
	BonusSystem bonusSystem;

	static const int NUMBER_OF_CHECKER_TYPES = 3;

	SDL_Texture* tableTexture = nullptr;
	SDL_Texture* backgroundTexture = nullptr;

	SDL_Texture* checkerTextures[NUMBER_OF_CHECKER_TYPES];
	SDL_Texture* gameTypeBanners = nullptr;

	SDL_Rect drawRect = { 0, 0, 0, 0 };
	SDL_Rect gameTypeBannerRect = { 0, 0, 0, 0 };
	SDL_Rect gameTypeBannerSrcRect = { 0, 0, 0, 0 };

	SDL_Rect positionRects[6][7] = { 0 };
	SDL_Rect positionDrawRects[6][7];

	CHECKER_TYPES grid[6][7] = { CHECKER_TYPES::NONE };
	Checker* animatedCheckers[6][7] = { 0 };
	PLAYERS m_turnPlayer = PLAYERS::NONE;

	double textureToRealRatio;
	int2 tableTexDims;

	bool allCheckersLanded = true;

	int getMouseColumn();
	int getMouseRow();
	int2 getEmptyPosition();
	int2 getEmptyPosition(int column);
	void detectWin();
	int2 dumbBotDecision();
	int2 smartBotDecision();
	PLAYERS getCheckerPlayer(CHECKER_TYPES checker);
	CHECKER_TYPES getPlayerChecker(PLAYERS player);
	int getPlayerNumber(PLAYERS player);
	int getCheckerNumber(CHECKER_TYPES checker);
	void placeChecker(int2 pos, CHECKER_TYPES checker);
	void deleteChecker(int2 pos);
	void handleUserClick();
};