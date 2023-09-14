#include <windows.h>
#include <SDL.h>

bool running = true;

struct RENDERSTATE
{
	int height, width, size;
	void* memory;

	BITMAPINFO bitmapInfo;
};

RENDERSTATE renderState;

#include "renderer.cpp"

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(69);
		case WM_DESTROY:
		{
			running = false;
		}break;
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect); //Get windows screen cords
			renderState.width = rect.right - rect.left; //width to renderState.
			renderState.height = rect.bottom - rect.top; //height to renderState.

			renderState.size = renderState.width * renderState.height * sizeof(unsigned int); //renderState.  Size will be the width * height * the size of an unsigned int

			if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE); //If the renderState.memory has information clear it
			renderState.memory = VirtualAlloc(0, renderState.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); //Store renderState.Size into renderState. memory
			//Bitmaps are like a grid that will store information per pixel
			//Defining the bitmap structure bitmapInfoHeader bmiHeader
			renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
			renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
			renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
			renderState.bitmapInfo.bmiHeader.biPlanes = 1;
			renderState.bitmapInfo.bmiHeader.biBitCount = 32; //32 bit unsigned int
			renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;

		}break;
		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
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
		BOOL mResult;
		while ( (mResult = PeekMessage(&message, window, 0, 0, PM_REMOVE)))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		switch (mResult)
		{
			case -1:
			{
				return -1;
			}break;
		}

		

		//Simulate
		clearScreen(0x0);
		drawRect(testX, testY, testX+40, testY+40, 0x69420);

		//Render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}