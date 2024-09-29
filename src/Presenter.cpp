#include "Presenter.h"
#include "InputManager.h"

Presenter* Presenter::m_instance = nullptr;

extern InputManager iptm;

SDL_Renderer* Presenter::getRenderer() const {
    return m_main_renderer;
}

void Presenter::init(string gameName) {
    m_SCREEN_WIDTH = 1920;
    m_SCREEN_HEIGHT = 1080;

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    m_main_window = SDL_CreateWindow(gameName.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1366, 768, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window,
        -1, SDL_RENDERER_PRESENTVSYNC);
    improveRenderer();

    m_instance = this;

    m_numberDrawer.init();
}

void Presenter::improveRenderer() {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    auto desktopWidth = DM.w;
    auto desktopHeight = DM.h;

    float2 mouseMultiply;
    mouseMultiply.x = (float)((double)m_SCREEN_WIDTH / (double)desktopWidth);
    mouseMultiply.y = (float)((double)m_SCREEN_HEIGHT / (double)desktopHeight);

    if (SDL_SetWindowFullscreen(m_main_window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
    {
        cout << "SDL_IMPROVE_RENDERER FAILED: %s\n" << SDL_GetError() << endl;
    }

    iptm.setMouseMultiply(mouseMultiply);

    SDL_RenderSetLogicalSize(m_main_renderer, m_SCREEN_WIDTH, m_SCREEN_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

void Presenter::draw() {
    SDL_RenderPresent(m_main_renderer);

    SDL_RenderClear(m_main_renderer);
}

SDL_Texture* Presenter::loadTexture(string bmpFile) {
    SDL_Texture* texture = nullptr;

    try {
        bmpFile = IMG_FOLDER + bmpFile;
        SDL_Surface* loadingSurface = SDL_LoadBMP(bmpFile.c_str());
        texture = SDL_CreateTextureFromSurface(m_main_renderer, loadingSurface);
        SDL_FreeSurface(loadingSurface);

        if (texture == nullptr) {
            throw (-1);
        }
    }
    catch (int i) {
        if (i < 0) {
            cerr << "NO TEXTURE LOADED: " << bmpFile << endl;

            bmpFile = IMG_FOLDER + "textureNotLoaded.bmp";

            SDL_Surface* loadingSurface = SDL_LoadBMP(bmpFile.c_str());
            texture = SDL_CreateTextureFromSurface(m_main_renderer, loadingSurface);
            SDL_FreeSurface(loadingSurface);
        }
    }

    return texture;
}

void Presenter::drawObject(SDL_Texture* texture) {
    SDL_Rect movedBkgRect = {
        0, 0,
        m_SCREEN_WIDTH, m_SCREEN_HEIGHT
    };
    if (cameraShiftActive) {
        movedBkgRect.x -= cameraPos.x;
        movedBkgRect.y -= cameraPos.y;
    }
    SDL_RenderCopy(m_main_renderer, texture, NULL, &movedBkgRect);
}

void Presenter::drawObject(SDL_Texture* texture, SDL_Rect location) {
    SDL_Rect movedRect = location;
    if (cameraShiftActive) {
        movedRect.x -= cameraPos.x;
        movedRect.y -= cameraPos.y;
    }
    SDL_RenderCopy(m_main_renderer, texture, NULL, &movedRect);
}

void Presenter::drawObject(SDL_Texture* texture, RotatedRect location) {
    SDL_Rect movedRect = location.toUnrotatedSdlRect();
    if (cameraShiftActive) {
        movedRect.x -= cameraPos.x;
        movedRect.y -= cameraPos.y;
    }
    double angleDegrees = location.angle * (180 / M_PI);
    SDL_RenderCopyEx(m_main_renderer, texture, NULL, &movedRect, angleDegrees, NULL, location.flip);
}

void Presenter::destruct() {
    m_numberDrawer.destruct();

    SDL_DestroyRenderer(m_main_renderer);
    SDL_DestroyWindow(m_main_window);
}