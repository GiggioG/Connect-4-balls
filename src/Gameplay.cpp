#include <utility>
#include "Gameplay.h"
#include "Presenter.h"
#include "ConfigManager.h"
#include "InputManager.h"
#include "Engine.h"

extern InputManager iptm;

Gameplay::Gameplay() {}
Gameplay::~Gameplay() {}

void Gameplay::init() {
	tableTexture = loadTexture("table.bmp");
	backgroundTexture = loadTexture("background2.bmp");
	checkerTextures[0] = loadTexture("football.bmp");
	checkerTextures[1] = loadTexture("basketball.bmp");
	checkerTextures[2] = loadTexture("stone.bmp");

	if (gameType == GAME_TYPE::MADNESS_MULTIPLAYER) { bonusSystem.init(this); }

	m_turnPlayer = PLAYERS::PLAYER_ONE;
	allCheckersLanded = true;

	int2 screenDims = getScreenDims();
	SDL_QueryTexture(tableTexture, nullptr, nullptr, &tableTexDims.x, &tableTexDims.y);
    double ratio = tableTexDims.y /(double)tableTexDims.x;
	drawRect = {
		screenDims.x / 2 - cfg("TABLE_WIDTH").i / 2,
		screenDims.y / 2 - (int)(cfg("TABLE_WIDTH").i * ratio) / 2,
		cfg("TABLE_WIDTH").i,
		(int)(cfg("TABLE_WIDTH").i * ratio)
	};

	textureToRealRatio = drawRect.w / (double)tableTexDims.x;
	int2 gridStart = {
		drawRect.x + (int)(68 * textureToRealRatio),
		drawRect.y + (int)(45 * textureToRealRatio)
	};

	SDL_Rect positionRect = { 
		gridStart.x, gridStart.y, 
		(int)(211 * textureToRealRatio), (int)(198 * textureToRealRatio)
	};
	SDL_Rect positionDrawRect = {
		0, 0,
		(int)(156 * textureToRealRatio), (int)(156 * textureToRealRatio)
	};

	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			positionDrawRect.x = positionRect.x + positionRect.w / 2 - positionDrawRect.w / 2;
			positionDrawRect.y = positionRect.y + positionRect.h / 2 - positionDrawRect.h / 2;

			positionDrawRects[r][c] = positionDrawRect;
			positionRects[r][c] = positionRect;
			grid[r][c] = CHECKER_TYPES::NONE;

			positionRect.x += positionRect.w;
		}
		positionRect.y += positionRect.h;
		positionRect.x = gridStart.x;
	}
}

int Gameplay::getMouseColumn() {
    if(iptm.m_mouseCoor.y - drawRect.y < 45 * textureToRealRatio || iptm.m_mouseCoor.y - drawRect.y > 1234 * textureToRealRatio) { return -1; }
	// cout << "in   ";
	int res = (int)(iptm.m_mouseCoor.x - drawRect.x - 68 * textureToRealRatio) / (int)(211 * textureToRealRatio);
    // cout << res << "\n";
	return (res < 0 || res > 6) ? -1 : res;
}
int Gameplay::getMouseRow() {
    if(iptm.m_mouseCoor.x - drawRect.x < 67 * textureToRealRatio || iptm.m_mouseCoor.x - drawRect.x > 1538 * textureToRealRatio) { return -1; }
	int res = (int)(iptm.m_mouseCoor.y - drawRect.y - 45 * textureToRealRatio) / (int)(198 * textureToRealRatio);
	return (res < 0 || res > 5) ? -1 : res;
}

int2 Gameplay::getEmptyPosition() {
    int column = getMouseColumn();
    if(column == -1){ return {-1, -1}; }
    int row = -2;
    for(int r=0; r<6; r++){
        if(grid[r][column] == CHECKER_TYPES::NONE){
            continue;
        }else{
            row = r-1;
			break;
        }
    }
    if(row == -2){
        row = 5;
    }
	if (row == -1) { return { -1, -1 }; }
    return {column, row};
}
int2 Gameplay::getEmptyPosition(int column) {
    if(column == -1){ return {-1, -1}; }
    int row = -2;
    for(int r=0; r<6; r++){
        if(grid[r][column] == CHECKER_TYPES::NONE){
            continue;
        }else{
            row = r-1;
			break;
        }
    }
    if(row == -2){
        row = 5;
    }
    if(row == -1){ return {-1, -1}; }
    return {column, row};
}

void Gameplay::detectWin() {
	/// horizontal win and draw detection
	bool hasEmpty = false;
	for (int r = 0; r < 6; r++) {
		CHECKER_TYPES runType = CHECKER_TYPES::NONE;
		int run = 0;
		for (int c = 0; c < 7; c++) {
			if (grid[r][c] == CHECKER_TYPES::NONE) { hasEmpty = true; }
			if (grid[r][c] == runType) { run++; }
			else {
				runType = grid[r][c];
				run = 1;
			}

			if (run == 4) {
				if (runType == CHECKER_TYPES::PLAYER_ONE) { m_winner = PLAYERS::PLAYER_ONE; return; }
				if (runType == CHECKER_TYPES::PLAYER_TWO) { m_winner = PLAYERS::PLAYER_TWO; return; }
			}
		}
	}
	if (!hasEmpty) { m_winner = PLAYERS::TIE; return; }
	/// vertical win detection
	for (int c = 0; c < 7; c++) {
		CHECKER_TYPES runType = CHECKER_TYPES::NONE;
		int run = 0;
		for (int r = 0; r < 6; r++) {
			if (grid[r][c] == CHECKER_TYPES::NONE) { hasEmpty = true; }
			if (grid[r][c] == runType) { run++; }
			else {
				runType = grid[r][c];
				run = 1;
			}

			if (run == 4) {
				if (runType == CHECKER_TYPES::PLAYER_ONE) { m_winner = PLAYERS::PLAYER_ONE; return; }
				if (runType == CHECKER_TYPES::PLAYER_TWO) { m_winner = PLAYERS::PLAYER_TWO; return; }
			}
		}
	}
	/// updards (/) diagonal win detection
	for (int diag = 0; diag < 12; diag++) {
		int2 curr;
		if (diag < 6) { curr = { 0, diag }; }
		else { curr = { diag - 5, 5 }; }

		CHECKER_TYPES runType = CHECKER_TYPES::NONE;
		int run = 0;
		for (; curr.x < 7 && curr.y >= 0; curr += {1, -1}) {
			const int r = curr.y, c = curr.x;
			if (grid[r][c] == CHECKER_TYPES::NONE) { hasEmpty = true; }
			if (grid[r][c] == runType) { run++; }
			else {
				runType = grid[r][c];
				run = 1;
			}

			if (run == 4) {
				if (runType == CHECKER_TYPES::PLAYER_ONE) { m_winner = PLAYERS::PLAYER_ONE; return; }
				if (runType == CHECKER_TYPES::PLAYER_TWO) { m_winner = PLAYERS::PLAYER_TWO; return; }
			}
		}
	}
	/// downwards (\) diagonal win detection
	for (int diag = 0; diag < 12; diag++) {
		int2 curr;
		if (diag < 6) { curr = { 0, 5-diag }; }
		else { curr = { diag-5, 0 }; }

		CHECKER_TYPES runType = CHECKER_TYPES::NONE;
		int run = 0;
		for (; curr.x < 7 && curr.y >= 0; curr += {1, 1}) {
			const int r = curr.y, c = curr.x;
			if (grid[r][c] == CHECKER_TYPES::NONE) { hasEmpty = true; }
			if (grid[r][c] == runType) { run++; }
			else {
				runType = grid[r][c];
				run = 1;
			}

			if (run == 4) {
				if (runType == CHECKER_TYPES::PLAYER_ONE) { m_winner = PLAYERS::PLAYER_ONE; return; }
				if (runType == CHECKER_TYPES::PLAYER_TWO) { m_winner = PLAYERS::PLAYER_TWO; return; }
			}
		}
	}
}

CHECKER_TYPES Gameplay::getPlayerChecker(PLAYERS player) {
	if (player == PLAYERS::PLAYER_ONE) { return CHECKER_TYPES::PLAYER_ONE; }
	if (player == PLAYERS::PLAYER_TWO) { return CHECKER_TYPES::PLAYER_TWO; }
}

int Gameplay::getPlayerNumber(PLAYERS player) {
	return (int)player - 1;
}

int Gameplay::getCheckerNumber(CHECKER_TYPES checker){
	return (int)checker - 1;
}

void Gameplay::placeChecker(int2 pos, CHECKER_TYPES checker){
	grid[pos.y][pos.x] = checker;
	Checker* newChecker = new Checker;
	newChecker->init(checkerTextures[getCheckerNumber(checker)], drawRect.y, pos, positionDrawRects);
	animatedCheckers[pos.y][pos.x] = newChecker;
}

void Gameplay::deleteChecker(int2 pos) {
	grid[pos.y][pos.x] = CHECKER_TYPES::NONE;
	animatedCheckers[pos.y][pos.x]->destroy();
	delete animatedCheckers[pos.y][pos.x];
	animatedCheckers[pos.y][pos.x] = nullptr;
}

void Gameplay::handleUserClick() {
	int2 pos = { getMouseColumn(), getMouseRow() };

	BONUS_RESULT bonusResult = bonusSystem.tryUseBonus(pos);
	if (bonusResult == BONUS_RESULT::FAIL) { return; }
	if (bonusResult == BONUS_RESULT::DIDNT_USE || bonusSystem.shouldGiveBonusTurn()) {
		pos = getEmptyPosition();
		if (pos == int2{ -1, -1 }) { return;  }
		placeChecker(pos, getPlayerChecker(m_turnPlayer));
	}

	if (!bonusSystem.shouldGiveBonusTurn()) {
		if (m_turnPlayer == PLAYERS::PLAYER_ONE) { m_turnPlayer = PLAYERS::PLAYER_TWO; }
		else if (m_turnPlayer == PLAYERS::PLAYER_TWO) { m_turnPlayer = PLAYERS::PLAYER_ONE; }
	}
}

PLAYERS Gameplay::getCheckerPlayer(CHECKER_TYPES checker) {
	if (checker == CHECKER_TYPES::PLAYER_ONE) { return PLAYERS::PLAYER_ONE; }
	if (checker == CHECKER_TYPES::PLAYER_TWO) { return PLAYERS::PLAYER_TWO; }
}

void Gameplay::update() {
	allCheckersLanded = true;
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			if (animatedCheckers[r][c] != nullptr) {
				animatedCheckers[r][c]->update(&allCheckersLanded);
			}
		}
	}

	if (allCheckersLanded) { detectWin(); }
	if (m_winner != PLAYERS::NONE) {
		if (m_winner == PLAYERS::TIE) {
			cout << "TIE!!!\n";
		}
		cout << "PLAYER " << getPlayerNumber(m_winner) + 1 << " WON!!!\n";
		return;
	}

	if((gameType == GAME_TYPE::NORMAL_WEAK_BOT || gameType == GAME_TYPE::NORMAL_STRONG_BOT) && m_turnPlayer == PLAYERS::PLAYER_TWO){
		if (!allCheckersLanded) { return; }
		
		int2 pos;
		if(gameType == GAME_TYPE::NORMAL_WEAK_BOT){
			pos = dumbBotDecision();
		}else{
			pos = smartBotDecision();
		}
		if (pos.x == -1) { return; }

		placeChecker(pos, getPlayerChecker(PLAYERS::PLAYER_TWO));

		if (m_turnPlayer == PLAYERS::PLAYER_ONE) { m_turnPlayer = PLAYERS::PLAYER_TWO; }
		else if (m_turnPlayer == PLAYERS::PLAYER_TWO) { m_turnPlayer = PLAYERS::PLAYER_ONE; }
	}
	if (iptm.anyKeyIsPressed() && gameType == GAME_TYPE::MADNESS_MULTIPLAYER) {
		if (!allCheckersLanded) { return; }
		bonusSystem.update();
	}
	if (iptm.m_mouseOnClick) {
		if (!allCheckersLanded) { return; }
		handleUserClick();
	}
}

void Gameplay::draw() {
	drawObject(backgroundTexture);

	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			if (animatedCheckers[r][c] != nullptr) {
				animatedCheckers[r][c]->draw();
			}
		}
	}
	drawObject(tableTexture, drawRect);

	if (gameType == GAME_TYPE::MADNESS_MULTIPLAYER) { bonusSystem.draw(); }

	//SDL_Texture* test = loadTexture("testSquare.bmp");
}

void Gameplay::destroy() {
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 7; c++) {
			if (animatedCheckers[r][c] != nullptr) {
				animatedCheckers[r][c]->destroy();
				delete animatedCheckers[r][c];
				animatedCheckers[r][c] = nullptr;
			}
		}
	}

	if (gameType == GAME_TYPE::MADNESS_MULTIPLAYER) { bonusSystem.destroy(); }

	SDL_DestroyTexture(tableTexture);
	SDL_DestroyTexture(backgroundTexture);
	for (int i = 0; i < NUMBER_OF_CHECKER_TYPES; i++) {
		SDL_DestroyTexture(checkerTextures[i]);
	}
}


int2 Gameplay::dumbBotDecision() {
	int2 res = {-1, -1};
	while(res == int2{-1, -1}){
		int c = (int)(randToOne()*7.0);
		res = getEmptyPosition(c);
	}
	return res;
}

int2 Gameplay::smartBotDecision() {

	for(int c=0; c<7; c++){
		int2 pos = getEmptyPosition(c);
		if (pos == int2{ -1, -1 }) { continue; }

		grid[pos.y][pos.x] = getPlayerChecker(PLAYERS::PLAYER_TWO);

		detectWin();
		grid[pos.y][pos.x] = CHECKER_TYPES::NONE;
		PLAYERS wouldBeWinner = m_winner;
		m_winner = PLAYERS::NONE;


		if(wouldBeWinner == PLAYERS::PLAYER_TWO){
			return pos;
		}
	}
	for(int c=0; c<7; c++){
		int2 pos = getEmptyPosition(c);
		if (pos == int2{ -1, -1 }) { continue; }

		grid[pos.y][pos.x] = getPlayerChecker(PLAYERS::PLAYER_ONE);

		detectWin();
		grid[pos.y][pos.x] = CHECKER_TYPES::NONE;
		PLAYERS wouldBeWinner = m_winner;
		m_winner = PLAYERS::NONE;

		if(wouldBeWinner == PLAYERS::PLAYER_ONE){
			return pos;
		}
	}

	return dumbBotDecision();
}