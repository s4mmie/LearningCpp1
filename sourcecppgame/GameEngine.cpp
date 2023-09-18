#pragma once
#include "GameEngine.h"
#include "GameTimer.h"


UINT8 red=0,green=255,blue=0;
Renderer r;
GameLoop loop;
Game g;
Text t;
int mapHeight = 4;
int mapWidth = 4;
std::vector<std::vector<char>> map(mapHeight, std::vector<char>(mapWidth, '#'));
std::string mapText = "";
SDL_Point mousePos;
bool leftMouseButtonDown = false;
SDL_Rect* selectedRect = NULL;
SDL_Point clickOffset;
SDL_Rect textBox = { 0, 0, 0, 0 };
std::vector<SDL_Rect*> rectangles;

SDL_Rect rect1 = { 288, 208, 100, 100 };
SDL_Rect rect2 = { 50, 50, 100, 80 };

void Game::Start()
{
	running = true;
	r.sdlWindow = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	r.sdlRender = SDL_CreateRenderer(r.sdlWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	rectangles.push_back(&textBox);
	rectangles.push_back(&rect1);
	rectangles.push_back(&rect2);
	loop.Update();
}

void Game::Stop()
{

	
	SDL_DestroyWindow(r.sdlWindow);
	SDL_DestroyRenderer(r.sdlRender);
	r.sdlWindow = nullptr;
	r.sdlRender = nullptr;

	SDL_Quit();
	running = false;
}

void Game::HandleEvents()
{
	SDL_Event sdlEvent;
	SDL_PollEvent(&sdlEvent);

	switch (sdlEvent.type)
	{
	case SDL_QUIT:
		Stop();
		break;

	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { running = false; break; }

	case SDL_MOUSEBUTTONDOWN:
		if (!leftMouseButtonDown && sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			leftMouseButtonDown = true;

			for (auto rect : rectangles)
			{
				if (SDL_PointInRect(&mousePos, rect))
				{
					if (rect == &textBox)
					{
						blue = 255;
						green = 0;
					}

					selectedRect = rect;
					clickOffset.x = mousePos.x - rect->x;
					clickOffset.y = mousePos.y - rect->y;
					break;
				}
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (leftMouseButtonDown && sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			leftMouseButtonDown = false;
			selectedRect = NULL;
			blue = 0;
			green = 255;
		}
		break;

	case SDL_MOUSEMOTION:
	{
		mousePos = { sdlEvent.motion.x, sdlEvent.motion.y };

		if (leftMouseButtonDown && selectedRect != NULL)
		{
			selectedRect->x = mousePos.x - clickOffset.x;
			selectedRect->y = mousePos.y - clickOffset.y;
		}
	}
	break;

	default: break;
	}
}

void GameLoop::Update()
{
	while (g.running)
	{
		g.HandleEvents();

		r.Render();
	}
}

void Renderer::Render()
{


	SDL_RenderClear(sdlRender);
	t.textTexture = t.Init(r.sdlRender, "res/arial.ttf", 12, mapText, { red, green, blue }, textBox.x, textBox.y, &textBox);
	t.display(textBox.x, textBox.y, r.sdlRender, t.textTexture, &textBox);
	for (auto const& rect : rectangles)
	{
		if (rect != &textBox)
		{
			SDL_SetRenderDrawColor(r.sdlRender, 0, 255, 0, 255);
			if (rect == selectedRect)
			{
				SDL_SetRenderDrawColor(r.sdlRender, 0, 0, 255, 255);
			}
			SDL_RenderFillRect(r.sdlRender, rect);
		}

	}
	SDL_SetRenderDrawColor(sdlRender, 0, 0, 0, 255);
	SDL_DestroyTexture(t.textTexture);
	SDL_RenderPresent(sdlRender);

}

SDL_Texture* Text::Init(SDL_Renderer* renderer,const std::string &fontPath, int fontSize, std::string& textMessage, const SDL_Color &color, int x,int y, SDL_Rect* rect)
{
	textTexture = loadFont(renderer, fontPath, fontSize, textMessage, color, x, rect);
	SDL_QueryTexture(textTexture, nullptr, nullptr, &textBox.w, &textBox.h);
	//rect->w = 100;
	//rect->h = 100;
	//rect->x = x;
	//rect->y = y;
	return textTexture;
}

void Text::display(int x, int y, SDL_Renderer* renderer, SDL_Texture* textTexture, SDL_Rect* rect) 
{
	rect->x = x;
	rect->y = y;
	SDL_RenderCopy(renderer, textTexture, nullptr, rect);

}

SDL_Texture* Text::loadFont(SDL_Renderer* renderer, const std::string &fontPath, int fontSize, const std::string &textMessage, const SDL_Color &color, int x, SDL_Rect* rect)
{
	UINT32 width = SCREEN_WIDTH - x;
	TTF_Font* font;
	SDL_Surface* textSurface;
	font = TTF_OpenFont(fontPath.c_str(), fontSize);

	textSurface = TTF_RenderText_Solid_Wrapped(font, textMessage.c_str(), color, width);

	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
	return textTexture;
}


int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdLine, int cmdShow)
{

	int xInt = -1;
	std::string append;
	for (auto x : map)
	{
		xInt++;
		int yInt = -1;
		for (auto y : x)
		{
			yInt++;
			append = map[xInt][yInt];
			mapText.append(append);
			mapText.append("  ");
		}
		mapText.append("\n");
	}



	g.Start();
	return 1;
}

/*
* struct RENDERSTATE
{
	int height, width, size;
	void* memory;

	BITMAPINFO bitmapInfo;
};

RENDERSTATE renderState;
* 
LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(69);
			running = false; break;
		case WM_DESTROY:
			g.Stop(); break;

		default: 
			DefWindowProc(hwnd, uMsg, wParam, lParam); break;

	}
}

const auto pClassName = L"source window";//pointer Class name
	//Create window class
	WNDCLASS wc = {};
	wc.style = CS_OWNDC;
	wc.lpszClassName = pClassName;
	wc.lpfnWndProc = windowCallback;

	//Register class
	RegisterClass(&wc);

	//Ceate window
	HWND window = CreateWindow(wc.lpszClassName, L"My First Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	int testX=0, testY=0;
	
	while (running)
	{	
		int testNum = 50;
		testX+=2;
		testY++;
		if (testX >= (1228 - testNum))
		{
			testX = 0;
		}
		if (testY >= (668 - testNum))
		{
			testY = 0;
		}

		//Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		

		//Simulate
		clearScreen(0x0);
		drawRect(testX, testY, testX+40, testY+40, 0x69420);

		//Render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}*/