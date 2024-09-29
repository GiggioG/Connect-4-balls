#pragma once

#include <utility>

#include <SDL.h>
#include <fstream>
#include <string>

#include "util.h"

class InputManager
{
public:
    InputManager();
    ~InputManager();

    bool m_mouseIsPressed = false;
    bool m_mouseIsDoubleClicked = false;
    bool m_mouseOnClick = false;  // mouse is being pressed for first time
    bool m_mouseOnRelease = false;
    bool m_mouseIsPressedPrevFrame = false;
    bool m_mouseIsHolded = false;
    bool m_mouseIsRightPressed = false;

    int2 m_mouseCoor = {0, 0};

    const Uint8* m_keyboardState = { 0 };

    int m_scroll = 0;

    void init();
    void update(); //< call the SDL_PollEvent fnc and update all variables
    void setMouseMultiply(float2 multiplier); //< multiply the mouseCoor corespondingly to the screen resolution
    void destruct();

    bool anyKeyIsPressed() const;
    bool keyIsPressed(SDL_Scancode key) const;

private:

    SDL_Event m_event = {};

    float2 m_mouseMultiply = {1, 1};
};