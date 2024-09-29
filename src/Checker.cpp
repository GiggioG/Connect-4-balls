#include "Checker.h"
#include "Gameplay.h"
#include "Presenter.h"

Checker::Checker() {}
Checker::~Checker() {}

void Checker::init(SDL_Texture* _texture, int _startY, int2 _pos, SDL_Rect _positionDrawRects[6][7]) {
	positionDrawRects = _positionDrawRects;
	texture = _texture;
	pos = _pos;
	rect = positionDrawRects[pos.y][pos.x];
	rect.y = _startY;
}

void Checker::update(bool* allCheckersLanded) {
	if (rect.y < positionDrawRects[pos.y][pos.x].y) {
		rect.y += 20;
		reachedTarget = false;
	} else {
		if (!reachedTarget) {
			 /// TODO: play sound
		}
		rect.y = positionDrawRects[pos.y][pos.x].y;
		reachedTarget = true;
	}

	*allCheckersLanded &= reachedTarget;
}

void Checker::draw() const {
	drawObject(texture, rect);
}

void Checker::destroy() {}