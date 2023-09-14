void renderBackground()
{
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int y = 0; y < renderState.height; y++)
	{
		for (int x = 0; x < renderState.width; x++)
		{
			*pixel++ = x + y;
		}
	}
}

void clearScreen(unsigned int color)
{
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int y = 0; y < renderState.height; y++)
	{
		for (int x = 0; x < renderState.width; x++)
		{
			*pixel++ = color;
		}
	}
}

void drawRect(int x1, int y1, int x2, int y2, unsigned int color)
{
	for (int y = y1; y < y2; y++)
	{
		unsigned int* pixel = (unsigned int*)renderState.memory + x1 + y * renderState.width;
		for (int x = x1; x < x2; x++)
		{
			*pixel++ = color;
		}
	}
}