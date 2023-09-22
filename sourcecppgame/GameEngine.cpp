#pragma once
#include "GameEngine.h"
#include "GameTimer.h"


UINT8 red = 0, green = 255, blue = 0, alpha = 255;
UINT32 startTime, frameTime;
Renderer r;
GameLoop loop;
Game g;
Text t;
int mapHeight = 16;
int mapWidth = 16;
float fps;
std::vector<std::vector<char>> map(mapHeight, std::vector<char>(mapWidth, '#'));
std::string mapText = "";
std::string fpsText = "0.0";
SDL_Point mousePos;
bool leftMouseButtonDown = false;
SDL_Rect* selectedRect = NULL;
SDL_Point clickOffset;
std::vector<SDL_Rect*> rectangles;
SDL_Color col;
SDL_Rect rect1 = { 288, 208, 100, 100 };
SDL_Rect rect2 = { 50, 50, 100, 80 };
SDL_Rect textBox = { 0, 0, 0, 0 };
SDL_Rect fpsBox = { 500, 100, 0, 0 };

unsigned int a = SDL_GetTicks();
unsigned int b = SDL_GetTicks();
double delta = 0;

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
	rectangles.push_back(&fpsBox);
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
	{/*
		startTime = SDL_GetTicks();
		g.HandleEvents();

		r.Render();
		fps = SDL_GetTicks() - startTime;
		fpsText = std::to_string(fps);
		//SDL_Delay(floor(16.666f - fps));
		*/
		a = SDL_GetTicks();
		delta = a - b;

		if (delta > 1000 / 60.0)
		{
			std::cout << "fps: " << 1000 / delta << std::endl;

			b = a;
			fpsText = std::to_string(1000/delta);

			g.HandleEvents();
			r.Render();
		}


	}
}

void Renderer::Render()
{
	SDL_RenderClear(sdlRender);
	for (auto const& rect : rectangles)
	{
		if (rect == selectedRect)
		{
			green = 0;
			blue = 255;

		}
		
		if (rect != selectedRect)
		{
			green = 255;
			blue = 0;
			
		}
		if(rect == &textBox)
		{
			alpha = 0;
			
		}
		col = { red, green, blue, alpha };
		SDL_SetRenderDrawColor(r.sdlRender, col.r, col.g, col.b, col.a);
		if (rect == &textBox || rect == &fpsBox)
		{
			SDL_RenderDrawRect(r.sdlRender, rect);
		}
		else
		{
			SDL_RenderFillRect(r.sdlRender, rect);
		}

	}

	
	t.mapTexture = t.Init(r.sdlRender, "res/arial.ttf", 12, mapText, col, textBox.x, textBox.y, &textBox);
	t.display(textBox.x, textBox.y, r.sdlRender, t.mapTexture, &textBox);
	t.fpsTexture = t.Init(r.sdlRender, "res/arial.ttf", 40, fpsText, col, fpsBox.x, fpsBox.y, &fpsBox);
	t.display(fpsBox.x, fpsBox.y, r.sdlRender, t.fpsTexture, &fpsBox);

	SDL_SetRenderDrawColor(sdlRender, 0, 0, 0, 255);
	SDL_DestroyTexture(t.mapTexture);
	SDL_DestroyTexture(t.fpsTexture);
	SDL_RenderPresent(sdlRender);

}

SDL_Texture* Text::Init(SDL_Renderer* renderer,const std::string &fontPath, int fontSize, std::string& textMessage, const SDL_Color &color, int x,int y, SDL_Rect* rect)
{
	SDL_Texture* textTexture = loadFont(renderer, fontPath, fontSize, textMessage, color, x, rect);
	SDL_QueryTexture(textTexture, nullptr, nullptr, &rect->w, &rect->h);
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
	UINT32 width = 216;
	TTF_Font* font;
	SDL_Surface* textSurface;
	font = TTF_OpenFont(fontPath.c_str(), fontSize);

	textSurface = TTF_RenderText_Solid_Wrapped(font, textMessage.c_str(), color, width);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

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
		//mapText.append("\n");
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