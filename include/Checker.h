#pragma once
#include <SDL.h>
#include "util.h"

class Checker {
public:
	Checker();
	~Checker();

	int2 pos = { -1, -1 };
	SDL_Texture* texture;
	bool reachedTarget = false;

	void init(SDL_Texture* _texture, int _startY, int2 _pos, SDL_Rect _positionDrawRects[6][7]);
	void update(bool* allCheckersLanded);
	void draw() const;
	void destroy();
private:
	SDL_Rect rect;
	SDL_Rect (*positionDrawRects)[7];
};