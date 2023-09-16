#include "GameEngine.h"
#include "GameTimer.h"

Renderer r;
GameLoop loop;
Game g;

void Game::Start()
{
	running = true;
	r.sdlWindow = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
	r.sdlRender = SDL_CreateRenderer(r.sdlWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_SetRenderDrawColor(r.sdlRender, 0, 0, 0, 255);

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

	SDL_SetRenderDrawColor(sdlRender, 0, 255, 0, 255);

	SDL_RenderPresent(sdlRender);
}

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdLine, int cmdShow)
{
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