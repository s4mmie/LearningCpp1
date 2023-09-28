#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define PIXEL_SIZE 1

#define R 1 // red wall
#define G 2 // green wall
#define B 3 // blue wall
#define W 4 // normal wall
#define S -1 //player spawn

struct Renderer
{
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRender;
    void Render();
};

struct GameLoop
{
    //Game state
    public:
        void Update();
};

struct Controls
{
    public:
        bool HandleInput(Renderer& render, GameLoop& simulate);
};

struct Game
{
    GameLoop loop;
    Renderer render;
    Controls con;

    bool running;

    void Start();
    void Stop();
    void HandleEvents();
};

struct Text
{
   SDL_Texture* Init(SDL_Renderer *renderer, const std::string &fontPath, int fontSize, std::string &textMessage, const SDL_Color &color, int x, int y, SDL_Rect* rect);

    void display(int x, int y, SDL_Renderer *renderer, SDL_Texture* textTexture, SDL_Rect* rect);

    SDL_Texture* loadFont(SDL_Renderer *renderer, const std::string &fontPath, int fontSize, const std::string &textMessage, const SDL_Color &color, int rectX, SDL_Rect* rect);
   
    SDL_Texture* mapTexture = nullptr;
    SDL_Texture* fpsTexture = nullptr;
    SDL_Texture* lagTexture = nullptr;
};