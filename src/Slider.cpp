#include "Presenter.h"
#include "Slider.h"
#include "Engine.h"
#include "InputManager.h"
#include "ConfigManager.h"

extern InputManager iptm;
extern ConfigManager cfgm;
extern Presenter pres;

Slider::Slider() {}
Slider::~Slider() {}

void Slider::init(int2 coord, int width, int _min, int _max, int initialValue, int* _callback){
	rect.x = coord.x;
	rect.y = coord.y;
	rect.w = width;
	rect.h = 30;
	min = _min;
	max = _max;
	val = initialValue;
	callback = _callback;
}

void Slider::update() {
	thumbRect.w = 15;
	thumbRect.h = 40;
	thumbRect.x = rangeMap(min, max, rect.x + thumbRect.w / 2, rect.x + rect.w - thumbRect.w - thumbRect.w/2, val);
	thumbRect.y = rect.y - (thumbRect.h - rect.h) / 2;

	if (iptm.m_mouseIsHolded && coordInRect(iptm.m_mouseCoor, rect)) {
		val = rangeMap(rect.x + thumbRect.w, rect.x + rect.w - thumbRect.w, min, max, iptm.m_mouseCoor.x);
		if (val < min) { val = min; }
		else if (val > max) { val = max; }
	}

	if (callback) { *callback = val; }
}

void Slider::draw() {
	drawObject(bodyTexture, rect);
	drawObject(thumbTexture, thumbRect);

	drawObject(bodyTexture);

	drawNum(to_string(val), { rect.x + rect.w + 20, rect.y-1 }, 32);
}

void Slider::destruct() {

}