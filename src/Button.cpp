#include "Button.h"
#include "util.h"
#include "Presenter.h"
#include "Engine.h"
#include "InputManager.h"
#include "ConfigManager.h"

extern InputManager iptm;

Button::Button() {}
Button::~Button() {}

void Button::init(SDL_Texture* _texture, SDL_Rect _rect, bool* _callback) {
	texture = _texture;
	rect = _rect;
	callback = _callback;
}

void Button::update() {
	pressed = (iptm.m_mouseOnClick && coordInRect(iptm.m_mouseCoor, rect));
	if (callback) { *callback = pressed; }
}

void Button::draw() const {
	SDL_Rect rect2 = rect;
	if (coordInRect(iptm.m_mouseCoor, rect2)) {
		int growHoriz = (int)((double)rect2.w * (cfg("BUTTON_GROW_FACTOR").f - 1.0f));
		int growVert = (int)((double)rect2.h * (cfg("BUTTON_GROW_FACTOR").f - 1.0f));

		rect2.x -= growHoriz / 2;
		rect2.w += growHoriz;

		rect2.y -= growVert / 2;
		rect2.h += growVert;
	}
	drawObject(texture, rect2);
}

void Button::destruct() {}