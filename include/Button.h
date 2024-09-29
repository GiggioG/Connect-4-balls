#pragma once
#include <SDL.h>

class Button {
public:
	Button();
	~Button();

	SDL_Texture* texture = nullptr;
	SDL_Rect rect = {-1, -1, -1, -1};
	bool* callback;
	bool pressed;
	
	void init(SDL_Texture* _texture, SDL_Rect _rect, bool* _callback = nullptr);
	void update();
	void draw() const;
	void destruct();
};