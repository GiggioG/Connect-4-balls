#pragma once

enum class GAME_STATE {
	NONE = 0,
	START_MENU = 1,
	GAMEPLAY = 2,
	END_SCREEN = 3
};

class State {
public:
	State();
	~State();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
};