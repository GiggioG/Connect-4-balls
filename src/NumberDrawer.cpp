#include "NumberDrawer.h"
#include "Presenter.h"
#include <string>

NumberDrawer* NumberDrawer::m_instance = nullptr;

NumberDrawer::NumberDrawer() {}
NumberDrawer::~NumberDrawer() {}

void NumberDrawer::init() {
	for (int i = 0; i <= 9; i++) {
		std::string path = "numerals/n#.bmp";
		path[10] = i + '0';
		numerals[i] = loadTexture(path);
		SDL_QueryTexture(numerals[i], NULL, NULL, &numeralDims[i].x, &numeralDims[i].y);
	}
	decimalPoint = loadTexture("numerals/decimalPoint.bmp");
	SDL_QueryTexture(decimalPoint, NULL, NULL, &decimalPointDims.x, &decimalPointDims.y);

	m_instance = this;
}

void NumberDrawer::drawNum(std::string num, int2 pos, int height) {
	int currX = pos.x;
	for (int i = 0; i < num.size(); i++) {
		if (num[i] == '.') {
			double mul = (double)height / (double)decimalPointDims.y;
			SDL_Rect drawRect;
			drawRect.x = currX;
			drawRect.y = pos.y;
			drawRect.w = (int)(decimalPointDims.x * mul);
			drawRect.h = (int)(decimalPointDims.y * mul);
			drawObject(decimalPoint, drawRect);
			currX += drawRect.w;
			continue;
		}

		int d = num[i] - '0';
		double mul = (double)height / (double) numeralDims[d].y;

		SDL_Rect drawRect;
		drawRect.x = currX;
		drawRect.y = pos.y;
		drawRect.w = (int)(numeralDims[d].x * mul);
		drawRect.h = (int)(numeralDims[d].y * mul);
		drawObject(numerals[d], drawRect);
		currX += drawRect.w;
	}
}

void NumberDrawer::destruct() {
	for (int i = 0; i <= 9; i++) {
		SDL_DestroyTexture(numerals[i]);
	}
}