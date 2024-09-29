#pragma once 

#include <SDL.h>
#include <SDL_ttf.h>
#include "util.h"
#include "NumberDrawer.h"

using namespace std;

class Presenter {
public:
	SDL_Window* m_main_window;
	SDL_Renderer* m_main_renderer;
	static Presenter* m_instance;

	int m_SCREEN_WIDTH;
	int m_SCREEN_HEIGHT;

	void init(string gameName);
	void draw();
	void destruct();

	bool cameraShiftActive = false;
	int2 cameraPos = { 0, 0 };

	SDL_Renderer* getRenderer() const;
	SDL_Texture* loadTexture(string bmpFile);

	void drawObject(SDL_Texture* texture);
	void drawObject(SDL_Texture* texture, SDL_Rect location);
	void drawObject(SDL_Texture* texture, RotatedRect location);

private:
	NumberDrawer m_numberDrawer;
	void improveRenderer();
};

namespace {
	void drawObject(SDL_Texture* texture) {
		Presenter::m_instance->drawObject(texture);
	}
	void drawObject(SDL_Texture* texture, SDL_Rect location) {
		Presenter::m_instance->drawObject(texture, location);
	}
	void drawObject(SDL_Texture* texture, RotatedRect location) {
		Presenter::m_instance->drawObject(texture, location);
	}

	SDL_Texture* loadTexture(string path) {
		return Presenter::m_instance->loadTexture(path);
	}

	int2 getScreenDims() {
		return { Presenter::m_instance->m_SCREEN_WIDTH, Presenter::m_instance->m_SCREEN_HEIGHT };
	}
}