#pragma once
#include "Window.h"

class Renderer
{
private:

	Window* TheWindow;
	

public:

	Renderer();
	Renderer(HINSTANCE hInstance, int CommandShow);
	~Renderer();

	Window* ReturnWindow();

	
};

