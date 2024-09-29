#pragma once

#include "util.h"
#include <SDL.h>

class Slider {
public:
	Slider();
	~Slider();

	SDL_Texture* thumbTexture;
	SDL_Texture* bodyTexture;
	SDL_Rect rect;
	SDL_Rect thumbRect;
	int max, min;
	int val;
	int* callback;

	void init(int2 coord, int width, int _min, int _max, int initialValue, int* _callback = nullptr);
	void update();
	void draw();
	void destruct();
};