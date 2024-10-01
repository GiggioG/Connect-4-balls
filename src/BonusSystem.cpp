#include <queue>
#include "BonusSystem.h"
#include "Presenter.h"
#include "InputManager.h"
#include "Gameplay.h"

extern InputManager iptm;

BonusSystem::BonusSystem() {}
BonusSystem::~BonusSystem() {}

void BonusSystem::init(Gameplay* _gamePlay) {
	gamePlay = _gamePlay;

	bonusTextures[0] = loadTexture("bonus_turn.bmp");
	bonusTextures[1] = loadTexture("destroy_column.bmp");
	bonusTextures[2] = loadTexture("remove_one.bmp");
	bonusTextures[3] = loadTexture("stone_virus.bmp");

	bonusUnavailableTexture = loadTexture("bonusUnavailable.bmp");

	for (int i = 0; i < NUMBER_OF_BONUS_TYPES; i++) {
		usedBonuses[0][i] = false;
		usedBonuses[1][i] = false;
	}
}

void BonusSystem::update() {
	if (iptm.keyIsPressed(SDL_SCANCODE_1)) { selectedBonus = BONUS::BONUS_TURN; }
	if (iptm.keyIsPressed(SDL_SCANCODE_2)) { selectedBonus = BONUS::DESTROY_COLUMN; }
	if (iptm.keyIsPressed(SDL_SCANCODE_3)) { selectedBonus = BONUS::REMOVE_ONE; }
	if (iptm.keyIsPressed(SDL_SCANCODE_4)) { selectedBonus = BONUS::VIRUS; }
	if (iptm.keyIsPressed(SDL_SCANCODE_0)) { selectedBonus = BONUS::NONE; }

	if (selectedBonus != BONUS::NONE && usedBonuses[gamePlay->getPlayerNumber(gamePlay->m_turnPlayer)][getBonusNumber(selectedBonus)]) {
		selectedBonus = BONUS::NONE;
	}
}

bool BonusSystem::virus(int2 pos) {
	int row = pos.y;
	int column = pos.x;
	if (row == -1 || column == -1) { return false; }
	if (gamePlay->grid[row][column] == CHECKER_TYPES::NONE) { return false; }

	CHECKER_TYPES type = gamePlay->grid[row][column];

	vector<int2> forGigo;

	queue<int2> bfs;
	bfs.push({ column, row });

	int cnt = 0;
	while (cnt < 5 && !bfs.empty()) {
		int2 currPos = bfs.front(); bfs.pop();
		if (gamePlay->grid[currPos.y][currPos.x] != CHECKER_TYPES::STONE) {
			cnt++;
			forGigo.push_back(currPos);
			gamePlay->grid[currPos.y][currPos.x] = CHECKER_TYPES::STONE;
		}

		if (currPos.y - 1 >= 0 && gamePlay->grid[currPos.y - 1][currPos.x] == type) {
			bfs.push({ currPos.x, currPos.y - 1 });
		}
		if (currPos.y - 1 >= 0 && currPos.x + 1 < 7 && gamePlay->grid[currPos.y - 1][currPos.x + 1] == type) {
			bfs.push({ currPos.x + 1, currPos.y - 1 });
		}
		if (currPos.x + 1 < 7 && gamePlay->grid[currPos.y][currPos.x + 1] == type) {
			bfs.push({ currPos.x + 1, currPos.y });
		}
		if (currPos.y + 1 < 6 && currPos.x + 1 < 7 && gamePlay->grid[currPos.y + 1][currPos.x + 1] == type) {
			bfs.push({ currPos.x + 1, currPos.y + 1 });
		}
		if (currPos.y + 1 < 6 && gamePlay->grid[currPos.y + 1][currPos.x] == type) {
			bfs.push({ currPos.x, currPos.y + 1 });
		}
		if (currPos.y + 1 < 6 && currPos.x - 1 >= 0 && gamePlay->grid[currPos.y + 1][currPos.x - 1] == type) {
			bfs.push({ currPos.x - 1, currPos.y + 1 });
		}
		if (currPos.x - 1 >= 0 && gamePlay->grid[currPos.y][currPos.x - 1] == type) {
			bfs.push({ currPos.x - 1, currPos.y });
		}
		if (currPos.y - 1 >= 0 && currPos.x - 1 >= 0 && gamePlay->grid[currPos.y - 1][currPos.x - 1] == type) {
			bfs.push({ currPos.x - 1, currPos.y - 1 });
		}
	}
	if (forGigo.size() == 0) {
		return false;
	}
	turnToStone(forGigo);
	return true;
}
bool BonusSystem::destroyColumn(int column) {
	if (column == -1) { return false; }
	bool hadNonEmpty = false;
	for (int r = 0; r < 6; r++) {
		if (gamePlay->grid[r][column] != CHECKER_TYPES::NONE) { hadNonEmpty = true; }
		gamePlay->deleteChecker({ column, r });
	}
	return hadNonEmpty;
}
bool BonusSystem::destroyOneChecker(int2 pos) {
	int row = pos.y;
	int column = pos.x;

	if (column == -1 || row == -1) { return false; }
	if (gamePlay->grid[row][column] == CHECKER_TYPES::NONE) { return false; }

	gamePlay->deleteChecker(pos);

	for (int r = row-1; r >= 0; r--) {
		swap(gamePlay->grid[r+1][column], gamePlay->grid[r][column]);
		if (gamePlay->animatedCheckers[r][column] != nullptr) {
			gamePlay->animatedCheckers[r][column]->pos.y++;
		}
		swap(gamePlay->animatedCheckers[r + 1][column], gamePlay->animatedCheckers[r][column]);
	}
	return true;
}

BONUS_RESULT BonusSystem::tryUseBonus(int2 pos) {
	if (selectedBonus == BONUS::NONE) { return BONUS_RESULT::DIDNT_USE; }

	BONUS_RESULT ret = BONUS_RESULT::NONE;
	if (selectedBonus == BONUS::DESTROY_COLUMN) {
		ret = destroyColumn(pos.x)? BONUS_RESULT::SUCCESS : BONUS_RESULT::FAIL;
	} else if (selectedBonus == BONUS::REMOVE_ONE) {
		ret = destroyOneChecker(pos) ? BONUS_RESULT::SUCCESS : BONUS_RESULT::FAIL;
	} else if (selectedBonus == BONUS::VIRUS) {
		ret = virus(pos) ? BONUS_RESULT::SUCCESS : BONUS_RESULT::FAIL;
	} else if (selectedBonus == BONUS::BONUS_TURN) {
		bonusTurnFlag = 2;
		ret = BONUS_RESULT::SUCCESS;
	}
	if (ret == BONUS_RESULT::SUCCESS) {
		usedBonuses[gamePlay->getPlayerNumber(gamePlay->m_turnPlayer)][getBonusNumber(selectedBonus)] = true;
	}
	selectedBonus = BONUS::NONE;
	return ret;
}

bool BonusSystem::shouldGiveBonusTurn() {
	return (bonusTurnFlag-- > 0);
}

int BonusSystem::getBonusNumber(BONUS bonus) {
	return (int)bonus - 1;
}

void BonusSystem::turnToStone(vector<int2> coords) {
	for (const int2 c : coords) {
		gamePlay->grid[c.y][c.x] = CHECKER_TYPES::STONE;
		gamePlay->animatedCheckers[c.y][c.x]->rect.y -= 5;
		gamePlay->animatedCheckers[c.y][c.x]->texture = gamePlay->checkerTextures[gamePlay->getCheckerNumber(CHECKER_TYPES::STONE)];
	}
}

void BonusSystem::draw() {
	int x = 20;
	int selectedOffset = 40;

	if (gamePlay->m_turnPlayer == PLAYERS::PLAYER_TWO) {
		x = 1700;
		selectedOffset *= -1;
	}


	for (int i = 0; i < 4; i++) {
		SDL_Rect bonusRect = { x, 20 + i * (220), 200, 200 };
		if (getBonusNumber(selectedBonus) == i) { bonusRect.x += selectedOffset; }
		drawObject(bonusTextures[i], bonusRect);
		if (usedBonuses[gamePlay->getPlayerNumber(gamePlay->m_turnPlayer)][i]) {
			drawObject(bonusUnavailableTexture, bonusRect);
		}
	}
}

void BonusSystem::destroy() {
	for (int i = 0; i < NUMBER_OF_BONUS_TYPES; i++) {
		SDL_DestroyTexture(bonusTextures[i]);
	}
}