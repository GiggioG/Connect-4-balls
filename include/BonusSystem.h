#pragma once
#include <SDL.h>
#include <vector>
#include "util.h"
//#include "Gameplay.h"
class Gameplay; /// foreward declared, #include in BonusSystem.cpp

enum class BONUS {
	NONE = 0,
	BONUS_TURN = 1,
	DESTROY_COLUMN = 2,
	REMOVE_ONE = 3,
	VIRUS = 4
};

enum class BONUS_RESULT {
	NONE = 1,
	SUCCESS = 2,
	FAIL = 3,
	DIDNT_USE = 4
};

class BonusSystem {
public:
	BonusSystem();
	~BonusSystem();

	Gameplay* gamePlay;
	BONUS selectedBonus = BONUS::NONE;

	void init(Gameplay* _gamePlay);
	void update();
	BONUS_RESULT tryUseBonus(int2 pos);
	bool shouldGiveBonusTurn();
	void draw();
	void destroy();

private:
	static const int NUMBER_OF_BONUS_TYPES = 4;
	bool usedBonuses[2][NUMBER_OF_BONUS_TYPES] = { 0 };

	SDL_Texture* bonusTextures[NUMBER_OF_BONUS_TYPES];

	SDL_Texture* bonusUnavailableTexture = nullptr;

	bool virus(int2 pos);
	bool destroyColumn(int column);
	bool destroyOneChecker(int2 pos);
	int getBonusNumber(BONUS bonus);
	void turnToStone(vector<int2> coords);
	int bonusTurnFlag = 0;
};