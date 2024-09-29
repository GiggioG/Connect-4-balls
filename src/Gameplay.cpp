#include <utility>
#include "Gameplay.h"
#include "Presenter.h"
#include "ConfigManager.h"
#include "InputManager.h"
#include "Engine.h"
#include <queue>

extern InputManager iptm;

Gameplay::Gameplay() {}
Gameplay::~Gameplay() {}

void Gameplay::init() {
	tableTexture = loadTexture("table.bmp");
	backgroundTexture = loadTexture("background.bmp");
	checkerTextures[0] = loadTexture("football.bmp");
	checkerTextures[1] = loadTexture("basketball.bmp");
	checkerTextures[2] = loadTexture("stone.bmp");

	bonusTextures[0] = loadTexture("bonus_turn.bmp");
	bonusTextures[1] = loadTexture("destroy_column.bmp");
	bonusTextures[2] = loadTexture("remove_one.bmp");
	bonusTextures[3] = loadTexture("stone_virus.bmp");

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

int Gameplay::checkColumn() {
    if(iptm.m_mouseCoor.y - drawRect.y < 45 * textureToRealRatio || iptm.m_mouseCoor.y - drawRect.y > 1234 * textureToRealRatio) { return -1; }
	// cout << "in   ";
	int res = (int)(iptm.m_mouseCoor.x - drawRect.x - 68 * textureToRealRatio) / (int)(211 * textureToRealRatio);
    // cout << res << "\n";
	return (res < 0 || res > 6) ? -1 : res;
}
int Gameplay::checkRow() {
    if(iptm.m_mouseCoor.x - drawRect.x < 67 * textureToRealRatio || iptm.m_mouseCoor.x - drawRect.x > 1538 * textureToRealRatio) { return -1; }
	int res = (int)(iptm.m_mouseCoor.y - drawRect.y - 45 * textureToRealRatio) / (int)(198 * textureToRealRatio);
	return (res < 0 || res > 5) ? -1 : res;
}

bool Gameplay::virus(){
	int column = checkColumn();
	int row = checkRow();
	if(row == -1 || column == -1){ return false; }

	CHECKER_TYPES type = grid[row][column];

	vector<int2> forGigo;

	queue<int2> bfs;
	bfs.push({column, row});

	int cnt = 0;
	while(cnt < 5 && !bfs.empty()){
		cnt++;
		int2 currPos = bfs.front();
		forGigo.push_back(currPos);
		grid[currPos.y][currPos.x] = CHECKER_TYPES::STONE;

		if(currPos.y-1 >= 0 && grid[currPos.y-1][currPos.x] == type){
			bfs.push({currPos.x, currPos.y-1});
		}
		if(currPos.y-1 >= 0 && currPos.x+1 < 7 && grid[currPos.y-1][currPos.x+1] == type){
			bfs.push({currPos.x+1, currPos.y-1});
		}
		if(currPos.x+1 < 7 && grid[currPos.y][currPos.x+1] == type){
			bfs.push({currPos.x+1, currPos.y});
		}
		if(currPos.y+1 < 6 && currPos.x+1 < 7 && grid[currPos.y+1][currPos.x+1] == type){
			bfs.push({currPos.x+1, currPos.y+1});
		}
		if(currPos.y+1 < 6 && grid[currPos.y+1][currPos.x] == type){
			bfs.push({currPos.x, currPos.y+1});
		}
		if(currPos.y+1 < 6 && currPos.x-1 >= 0 && grid[currPos.y+1][currPos.x-1] == type){
			bfs.push({currPos.x-1, currPos.y+1});
		}
		if(currPos.x-1 >= 0 && grid[currPos.y][currPos.x-1] == type){
			bfs.push({currPos.x-1, currPos.y});
		}
		if(currPos.y-1 >= 0 && currPos.x-1 >= 0 && grid[currPos.y-1][currPos.x+1] == type){
			bfs.push({currPos.x+1, currPos.y-1});
		}
	}
	turnToStone(forGigo);
	
	
	return true;
}
bool Gameplay::destroyColumn() {
	int column = checkColumn();
	if(column == -1){ return false; }
	for(int r=0; r<6; r++){
		grid[r][column] = CHECKER_TYPES::NONE;
	}
	for(int i=0; i<checkers.size(); i++){
		if(checkers[i].pos.x == column){
			swap(checkers[i], checkers[checkers.size()-1]);
			checkers.pop_back();
			i--;
		}
	}
	return true;
}
bool Gameplay::destroyOneChecker() {
	int column = checkColumn();
	int row = checkRow();
	if(row == -1 || column == -1){ return false; }
	grid[row][column] = CHECKER_TYPES::NONE;
	for(int r=row; r>0; r--){
		swap(grid[r][column], grid[r-1][column]);
	}
	for(int i=0; i<checkers.size(); i++){
		if(checkers[i].pos.x == column && checkers[i].pos.y == row){
			swap(checkers[i], checkers[checkers.size()-1]);
			checkers.pop_back();
			i--;
			break;
		}
	}
	for(int i=0; i<checkers.size(); i++){
		if(checkers[i].pos.x == column && checkers[i].pos.y < row){
			checkers[i].pos = {column, row + 1};
		}
	}
	return true;
}

int2 Gameplay::playMove() {
    int column = checkColumn();
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
int2 Gameplay::playMove(int column) {
    if(column == -1){ return {-1, -1}; }
    int row = -2;
    for(int r=0; r<6; r++){
        if(grid[r][column] == CHECKER_TYPES::NONE){
            continue;
        }else{
            row = r-1;
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

int Gameplay::getCheckerNumber(CHECKER_TYPES checker){
	return (int)checker - 1;
}

int Gameplay::getBonusNumber(BONUS bonus){
	return (int)bonus - 1;
}

int Gameplay::getPlayerNumber(PLAYERS player){
	return (int)player - 1;
}

void Gameplay::turnToStone(vector<int2> coords) {
	for (const int2 c : coords) {
		grid[c.y][c.x] = CHECKER_TYPES::STONE;
	}

	for (Checker& checker : checkers) {
		for (const int2 p : coords) {
			if (checker.pos == p) {
				checker.texture = checkerTextures[2];
			}
		}
	}
}

void Gameplay::drawBonuses() {
	int x = 20;
	int selectedOffset = 30;

	if (m_turnPlayer == PLAYERS::PLAYER_TWO) {
		x = 1700;
		selectedOffset = -30;
	}

	
	for (int i = 0; i < 4; i++) {
		SDL_Rect bonusRect = { x, 20 + i*(220), 200, 200};
		if (getBonusNumber(currBonus) == i) { bonusRect.x += selectedOffset; }
		drawObject(bonusTextures[i], bonusRect);
	}
}

PLAYERS Gameplay::getCheckerPlayer(CHECKER_TYPES checker) {
	if (checker == CHECKER_TYPES::PLAYER_ONE) { return PLAYERS::PLAYER_ONE; }
	if (checker == CHECKER_TYPES::PLAYER_TWO) { return PLAYERS::PLAYER_TWO; }
}

void Gameplay::update() {
	allCheckersLanded = true;
	for (Checker& c : checkers) { c.update(&allCheckersLanded); }

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

		grid[pos.y][pos.x] = getPlayerChecker(m_turnPlayer);
		Checker newChecker;
		newChecker.init(checkerTextures[getCheckerNumber(grid[pos.y][pos.x])], drawRect.y, pos, positionDrawRects);
		checkers.push_back(newChecker);

		if (m_turnPlayer == PLAYERS::PLAYER_ONE) { m_turnPlayer = PLAYERS::PLAYER_TWO; }
		else if (m_turnPlayer == PLAYERS::PLAYER_TWO) { m_turnPlayer = PLAYERS::PLAYER_ONE; }
	}
	if (iptm.anyKeyIsPressed()) {
		if (!allCheckersLanded) { return; }
		if (iptm.keyIsPressed(SDL_SCANCODE_1)) { currBonus = BONUS::BONUS_TURN; }
		if (iptm.keyIsPressed(SDL_SCANCODE_2)) { currBonus = BONUS::DESTROY_COLUMN; }
		if (iptm.keyIsPressed(SDL_SCANCODE_3)) { currBonus = BONUS::REMOVE_ONE; }
		if (iptm.keyIsPressed(SDL_SCANCODE_4)) { currBonus = BONUS::VIRUS; }
	}
	if (iptm.m_mouseOnClick) {
		if (!allCheckersLanded) { return; }
		int2 pos = playMove();
		if (pos == int2{ -1, -1 }) { return; }
		
		if(currBonus == BONUS::DESTROY_COLUMN){
			destroyColumn();
		}else if(currBonus == BONUS::REMOVE_ONE){
			destroyOneChecker();
		}else if(currBonus == BONUS::VIRUS){
			virus();
		}else{
			grid[pos.y][pos.x] = getPlayerChecker(m_turnPlayer);
			Checker newChecker;
			newChecker.init(checkerTextures[getCheckerNumber(grid[pos.y][pos.x])], drawRect.y, pos, positionDrawRects);
			checkers.push_back(newChecker);
		}

		if(currBonus != BONUS::BONUS_TURN){
			if (m_turnPlayer == PLAYERS::PLAYER_ONE) { m_turnPlayer = PLAYERS::PLAYER_TWO; }
			else if (m_turnPlayer == PLAYERS::PLAYER_TWO) { m_turnPlayer = PLAYERS::PLAYER_ONE; }
		}
		
		currBonus = BONUS::NONE;
	}
}

void Gameplay::draw() {
	drawObject(backgroundTexture);

	for (const Checker& c : checkers) { c.draw(); }
	drawObject(tableTexture, drawRect);

	if (gameType == GAME_TYPE::MADNESS_MULTIPLAYER) {
		drawBonuses();
	}

	//SDL_Texture* test = loadTexture("testSquare.bmp");
}

void Gameplay::destroy() {
	for (Checker& c : checkers) {
		c.destroy();
	}
	checkers.clear();

	SDL_DestroyTexture(tableTexture);
	tableTexture = nullptr;
	SDL_DestroyTexture(backgroundTexture);
	backgroundTexture = nullptr;
	for (int i = 0; i < (sizeof(checkerTextures) / sizeof(SDL_Texture*)); i++) {
		SDL_DestroyTexture(checkerTextures[i]);
		checkerTextures[i] = nullptr;
	}
	for (int i = 0; i < (sizeof(bonusTextures) / sizeof(SDL_Texture*)); i++) {
		SDL_DestroyTexture(bonusTextures[i]);
		bonusTextures[i] = nullptr;
	}
}


int2 Gameplay::dumbBotDecision() {
	int2 res = {-1, -1};
	while(res == int2{-1, -1}){
		int c = (int)(randToOne()*7.0);
		res = playMove(c);
	}
	return res;
}

int2 Gameplay::smartBotDecision() {

	for(int c=0; c<7; c++){
		int2 pos = playMove(c);
		if (pos == int2{ -1, -1 }) { continue; }

		grid[pos.y][pos.x] = getPlayerChecker(PLAYERS::PLAYER_TWO);

		detectWin();
		PLAYERS wouldBeWinner = m_winner;
		m_winner = PLAYERS::NONE;


		if(wouldBeWinner == PLAYERS::PLAYER_TWO){
			return pos;
		}
	}
	for(int c=0; c<7; c++){
		int2 pos = playMove(c);
		if (pos == int2{ -1, -1 }) { continue; }

		grid[pos.y][pos.x] = getPlayerChecker(PLAYERS::PLAYER_ONE);

		detectWin();
		PLAYERS wouldBeWinner = m_winner;
		m_winner = PLAYERS::NONE;

		if(wouldBeWinner == PLAYERS::PLAYER_ONE){
			return pos;
		}
	}

	return dumbBotDecision();
}