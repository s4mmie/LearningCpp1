#pragma once
#include "GameEngine.h"
#include "GameTimer.h"

struct Player
{
	float x = 14.5, y = 2.2 , z;
	float pitch = 0.f, yaw = -0.9f, roll = 0.f; // .../img/PitchYawRoll Example.jpeg
	const float fov = 3.14 / 4;
};

UINT8 red = 0, green = 255, blue = 0, alpha = 255;


Renderer r;
GameLoop loop;
Game g;
Text t;
Player p;

std::wstring wMap;

const int mapHeight = 16, mapWidth = 16;
unsigned int frameStart = SDL_GetTicks(), frameTime = SDL_GetTicks();

float fps, fpsCap = 128.f;

bool leftMouseButtonDown = false, lagMode = false;

double delta = 0;

SDL_Color col = { 0, 0, 0, 255 };

std::vector<SDL_Rect*> rectangles;
std::vector<SDL_Point*> points;
std::vector < std::vector<SDL_Color> >screenColor(SCREEN_WIDTH/PIXEL_SIZE, std::vector<SDL_Color>(SCREEN_HEIGHT/ PIXEL_SIZE, col));
std::vector<std::vector<char>> map(mapHeight, std::vector<char>(mapWidth, '#'));
std::string mapText = "", fpsText = "", lagText = "LAG TURNED ON: 30 FPS";

SDL_Point clickOffset, mousePos;
SDL_Rect rect1 = { 288, 208, 100, 100 };
SDL_Rect rect2 = { 50, 50, 100, 80 };
SDL_Rect textBox = { 24, 123, 0, 0 };
SDL_Rect fpsBox = { 100, 100, 0, 0 };
SDL_Rect lagBox = { 0, 520, 0 ,0 };
SDL_Rect* selectedRect = NULL;

void Game::Start()
{
	running = true;
	r.sdlWindow = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	r.sdlRender = SDL_CreateRenderer(r.sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	rectangles.push_back(&rect1);
	rectangles.push_back(&rect2);
	rectangles.push_back(&textBox);
	rectangles.push_back(&fpsBox);
	loop.Update();
}

void Game::Stop()
{
	SDL_DestroyWindow(r.sdlWindow);
	SDL_DestroyRenderer(r.sdlRender);
	r.sdlWindow = nullptr;
	r.sdlRender = nullptr;

	running = false;
	SDL_Quit();

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
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_F) { fpsCap = 30.f; lagMode = true; break; }
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) { p.yaw -= 0.05f; break; }
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) { p.yaw += 0.05f; break; }

		//if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) { p.x -= sinf(p.yaw) * 0.2f; break; }
		//if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) { p.x += sinf(p.yaw) * 0.2f; break; }

		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) 
		{
			p.x += sinf(p.yaw) * 0.5f;
			p.y += cosf(p.yaw) * 0.5f; 
			break; 
		}
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) 
		{
			p.x -= sinf(p.yaw) * 0.5f;
			p.y -= cosf(p.yaw) * 0.5f; 
			break;
		}
		//Add key up dection for movement so i can hvae multiple keys being held down. have something that is called and will continue to run until key up happens
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_R) { p.y = 7; p.x = 5; p.yaw = 0.f; break; }
	case SDL_KEYUP:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_F) { fpsCap = 128.f; lagMode = false; break; }
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
	{

		for (int x = 0; x < (SCREEN_WIDTH / PIXEL_SIZE); x++)
		{
			float rayAngle = (p.yaw - p.fov / 2.0f) + ((float)x / float(SCREEN_WIDTH / PIXEL_SIZE) * p.fov);
			float distanceToWall = 0;

			bool isWallHit = false;

			float eyeX = sinf(rayAngle);
			float eyeY = cosf(rayAngle);

			while (!isWallHit && distanceToWall < mapWidth)
			{
				distanceToWall += 0.1f;
				int wTestX = (int)(p.x + eyeX * distanceToWall);//Wall Test X & Wall Test Y
				int wTestY = (int)(p.y + eyeY * distanceToWall);

				if (wTestX < 0 || wTestX >= mapWidth || wTestY < 0 || wTestY >= mapHeight)
				{
					isWallHit = true;
					distanceToWall = mapWidth;
				}
				else
				{
					if (wMap[wTestX * mapWidth + wTestY] == '#')
					{
						isWallHit = true;
					}
				}
			}
			int sCeiling = (float)((SCREEN_HEIGHT / PIXEL_SIZE) / 2.0) - (SCREEN_HEIGHT / PIXEL_SIZE) / ((float)distanceToWall); //Screen Ceiling & Screen Floor
			int sFloor = (SCREEN_HEIGHT / PIXEL_SIZE) - sCeiling;
			
			int distanceR, distanceG, distanceB, distanceAmount;
			if (distanceToWall <= mapWidth / 4) //close
			{
				distanceAmount = 200;
				distanceR = distanceAmount; distanceG = distanceAmount; distanceB = distanceAmount;
			}
			else if (distanceToWall < mapWidth / 3) //semi close
			{
				distanceAmount = 150;
				distanceR = distanceAmount; distanceG = distanceAmount; distanceB = distanceAmount;
			}
			else if (distanceToWall < mapWidth / 2) //semi far
			{
				distanceAmount = 100;
				distanceR = distanceAmount; distanceG = distanceAmount; distanceB = distanceAmount;
			}
			else if (distanceToWall < mapWidth) //far
			{
				distanceAmount = 50;
				distanceR = distanceAmount; distanceG = distanceAmount; distanceB = distanceAmount;
			}
			else // too far
			{
				distanceAmount = 0;
				distanceR = distanceAmount; distanceG = distanceAmount; distanceB = distanceAmount;
			}

			for (int y = 0; y < (SCREEN_HEIGHT / PIXEL_SIZE); y++)
			{
				if (y <= sCeiling)
				{
					red = 55; green = 55; blue = 55;
					col = { red, green, blue, alpha };
					screenColor[x][y] = col;
				}
				else if (y > sCeiling && y <= sFloor)
				{
					red = 55 + distanceR; green = 0; blue = 0;
					col = { red, green, blue, alpha };
					screenColor[x][y] = col;
				}
				else
				{
					red = 255; green = 182; blue = 192;
					col = { red, green, blue, alpha };
					screenColor[x][y] = col;
				}
				if (y == SCREEN_HEIGHT - 1)
				{
					int gotcha = 1;
				}
			}
			
		}
		frameStart = SDL_GetTicks();
		delta = frameStart - frameTime;
		if (delta > 1000/fpsCap)
		{
			std::string fpsTextBase = "FPS: ";
			
			fps = (delta > 0) ? 1000.0f / delta: 0.0f;

			std::string fpsTextFps = std::to_string(fps);

			fpsText = fpsTextBase + fpsTextFps;

			g.HandleEvents();
			r.Render();
			frameTime = frameStart;
		}


	}
}

void Renderer::Render()
{

	for(auto x = 0; x < (SCREEN_WIDTH / PIXEL_SIZE); x++)
	{
		for (auto y = 0; y < (SCREEN_HEIGHT / PIXEL_SIZE); y++)
		{
			SDL_RenderSetScale(sdlRender,PIXEL_SIZE, PIXEL_SIZE);
			SDL_SetRenderDrawColor(sdlRender, screenColor[x][y].r, screenColor[x][y].g, screenColor[x][y].b, 255);
			SDL_RenderDrawPoint(sdlRender, x, y);
		}
	}
	SDL_RenderSetScale(sdlRender, 1, 1);
	for (auto const& rect : rectangles)
		{
			if (rect == selectedRect)
			{
				red = 0; green = 0; blue = 255;
				if (selectedRect == &textBox)
				{
					red = 255; green = 0; blue = 255;
				}
				else if (selectedRect == &fpsBox)
				{
					red = 44; green = 101; blue = 121;
				}
			}
		
			if (rect != selectedRect)
			{
				red = 0; green = 255; blue = 0;
			}


			if (rect == &textBox)
			{
				col = { red, green, blue, alpha };
				SDL_SetRenderDrawColor(r.sdlRender, col.r, col.g, col.b, col.a);
				SDL_RenderDrawRect(r.sdlRender, rect);
			}
			else if (rect == &fpsBox)
			{
				col = { red, green, blue, alpha };
				SDL_SetRenderDrawColor(r.sdlRender, col.r, col.g, col.b, col.a);
				SDL_RenderDrawRect(r.sdlRender, rect);
			}
			else
			{
				col = { red, green, blue, alpha };
				SDL_SetRenderDrawColor(r.sdlRender, col.r, col.g, col.b, col.a);
				SDL_RenderFillRect(r.sdlRender, rect);
			}

		}
	if (selectedRect == &textBox)
	{

		red = 255; green = 0; blue = 255;
		col = { red, green, blue, alpha };
		t.mapTexture = t.Init(r.sdlRender, "res/arial.ttf", 12, mapText, col, textBox.x, textBox.y, &textBox);
		red = 0; blue = 0; green = 255;
		col = { red, green, blue, alpha };
		t.fpsTexture = t.Init(r.sdlRender, "res/arial.ttf", 40, fpsText, col, fpsBox.x, fpsBox.y, &fpsBox);
	}
	else if(selectedRect == &fpsBox)
	{
		red = 0; blue = 0; green = 255;
		col = { red, green, blue, alpha };
		t.mapTexture = t.Init(r.sdlRender, "res/arial.ttf", 12, mapText, col, textBox.x, textBox.y, &textBox);
		red = 44; green = 101; blue = 121;
		col = { red, green, blue, alpha };
		t.fpsTexture = t.Init(r.sdlRender, "res/arial.ttf", 40, fpsText, col, fpsBox.x, fpsBox.y, &fpsBox);
	}
	else
	{
		col = { red, green, blue, alpha };
		t.mapTexture = t.Init(r.sdlRender, "res/arial.ttf", 12, mapText, col, textBox.x, textBox.y, &textBox);
		t.fpsTexture = t.Init(r.sdlRender, "res/arial.ttf", 40, fpsText, col, fpsBox.x, fpsBox.y, &fpsBox);
	}
	if (lagMode == true)
	{
		red = 255; green = 0; blue = 0;
		col = { red, green, blue, alpha };
		t.lagTexture = t.Init(r.sdlRender, "res/arial.ttf", 45, lagText, col, lagBox.x, lagBox.y, &lagBox);
		t.display(lagBox.x, lagBox.y, r.sdlRender, t.lagTexture, &lagBox);
	}
	t.display(textBox.x, textBox.y, r.sdlRender, t.mapTexture, &textBox);
	t.display(fpsBox.x, fpsBox.y, r.sdlRender, t.fpsTexture, &fpsBox);
	
	SDL_SetRenderDrawColor(sdlRender, 0, 0, 0, 255);
	SDL_RenderPresent(sdlRender);


	SDL_DestroyTexture(t.mapTexture);
	SDL_DestroyTexture(t.fpsTexture);
	SDL_DestroyTexture(t.lagTexture);
	SDL_RenderClear(sdlRender);
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
		mapText.append("\n");
	}
	
	wMap += L"################";
	wMap += L"#..............#";
	wMap += L"#..######......#";
	wMap += L"#..............#";
	wMap += L"#.......##.....#";
	wMap += L"#.......##.....#";
	wMap += L"#..............#";
	wMap += L"#..............#";
	wMap += L"#..............#";
	wMap += L"#..............#";
	wMap += L"#..............#";
	wMap += L"#.......##.....#";
	wMap += L"#..##..........#";
	wMap += L"#..............#";
	wMap += L"#..............#";
	wMap += L"################";

	


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