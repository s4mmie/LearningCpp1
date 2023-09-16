#pragma once

#include <SDL.h>


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