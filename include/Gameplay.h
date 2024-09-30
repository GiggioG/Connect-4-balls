#pragma once
#include <vector>
#include <SDL.h>
#include "State.h"
#include "util.h"
#include "Checker.h"

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

enum class BONUS {
	NONE = 0,
	BONUS_TURN = 1,
	DESTROY_COLUMN = 2,
	REMOVE_ONE = 3,
	VIRUS = 4
};

class Gameplay : public State {
public:
	Gameplay();
	~Gameplay();

	PLAYERS m_winner = PLAYERS::NONE;
	GAME_TYPE gameType = GAME_TYPE::NONE;

	static const int NUMBER_OF_CHECKER_TYPES = 3;
	static const int NUMBER_OF_BONUS_TYPES = 4;

	void init();
	void update();
	void draw();
	void destroy();
private:
	bool usedBonuses[2][NUMBER_OF_BONUS_TYPES] = { 0 };
	BONUS currBonus = BONUS::NONE;

	SDL_Texture* tableTexture = nullptr;
	SDL_Texture* backgroundTexture = nullptr;

	SDL_Texture* bonusTextures[NUMBER_OF_BONUS_TYPES];
	SDL_Texture* checkerTextures[NUMBER_OF_CHECKER_TYPES];
	SDL_Texture* bonusUnavailableTexture = nullptr;

	SDL_Rect drawRect = { 0, 0, 0, 0 };

	SDL_Rect positionRects[6][7] = { 0 };
	SDL_Rect positionDrawRects[6][7];

	CHECKER_TYPES grid[6][7] = { CHECKER_TYPES::NONE };
	PLAYERS m_turnPlayer = PLAYERS::NONE;

	double textureToRealRatio;
	int2 tableTexDims;

	vector<Checker> checkers;
	bool allCheckersLanded = true;

	int checkColumn();
	int checkRow();
	bool virus();
	bool destroyColumn();
	bool destroyOneChecker();
	int2 getEmptyPosition();
	int2 getEmptyPosition(int column);
	void detectWin();
	int2 dumbBotDecision();
	int2 smartBotDecision();
	PLAYERS getCheckerPlayer(CHECKER_TYPES checker);
	CHECKER_TYPES getPlayerChecker(PLAYERS player);
	int getCheckerNumber(CHECKER_TYPES checker);
	int getBonusNumber(BONUS bonus);
	int getPlayerNumber(PLAYERS player);
	void turnToStone(vector<int2> coords);
	void drawBonuses();
};