#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>


#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define PIXEL_SIZE 5

class Renderer
{
public:
        SDL_Window* sdlWindow;
        SDL_Renderer* sdlRender;
        void Render();
};

class GameLoop
{
    //Game state
    public:
        void Update();
};

class Controls
{
    public:
        bool HandleInput(Renderer& render, GameLoop& simulate);
};

class Game
{
    GameLoop loop;
    Renderer render;
    Controls con;

public:
    bool running;

    void Start();
    void Stop();
    void HandleEvents();

    inline void Run()
    {
        while (con.HandleInput(render, loop))
        {
            loop.Update();
            render.Render();
        }
    }
};

class Text
{
public:

    SDL_Texture* Init(SDL_Renderer *renderer, const std::string &fontPath, int fontSize, std::string &textMessage, const SDL_Color &color, int x, int y, SDL_Rect* rect);

    void display(int x, int y, SDL_Renderer *renderer, SDL_Texture* textTexture, SDL_Rect* rect);

    SDL_Texture* loadFont(SDL_Renderer *renderer, const std::string &fontPath, int fontSize, const std::string &textMessage, const SDL_Color &color, int rectX, SDL_Rect* rect);
   
    SDL_Texture* mapTexture = nullptr;
    SDL_Texture* fpsTexture = nullptr;
    SDL_Texture* lagTexture = nullptr;
};