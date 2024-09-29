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

	BONUS currBonus = BONUS::NONE;

	PLAYERS m_winner = PLAYERS::NONE;
	GAME_TYPE gameType = GAME_TYPE::NONE;

	bool usedBonuses[2][4] = { 0 };

	SDL_Texture* tableTexture = nullptr;
	SDL_Texture* backgroundTexture = nullptr;

	SDL_Rect drawRect = { 0, 0, 0, 0 };

	SDL_Rect positionRects[6][7] = { 0 };
	SDL_Rect positionDrawRects[6][7];

	CHECKER_TYPES grid[6][7] = { CHECKER_TYPES::NONE };
	PLAYERS m_turnPlayer = PLAYERS::NONE;

	void init();
	void update();
	void draw();
	void destroy();
private:
	double textureToRealRatio;
	int2 tableTexDims;
	vector<Checker> checkers;
	int checkColumn();
	int checkRow();
	bool virus();
	bool destroyColumn();
	bool destroyOneChecker();
	int2 playMove();
	int2 playMove(int column);
	void detectWin();
	int2 dumbBotDecision();
	int2 smartBotDecision();
	PLAYERS getCheckerPlayer(CHECKER_TYPES checker);
	CHECKER_TYPES getPlayerChecker(PLAYERS player);
	int getCheckerNumber(CHECKER_TYPES checker);
	int getBonusNumber(BONUS bonus);
	int getPlayerNumber(PLAYERS player);
	void turnToStone(vector<int2> coords);
	bool allCheckersLanded = true;
	void drawBonuses();

	SDL_Texture* bonusTextures[4];
	SDL_Texture* checkerTextures[3];
};