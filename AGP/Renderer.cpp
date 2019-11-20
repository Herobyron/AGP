#include "Renderer.h"


Renderer::Renderer()
{

}

Renderer::Renderer(HINSTANCE hInstance, int CommandShow)
{
	TheWindow = new Window(hInstance, CommandShow);

}


Renderer::~Renderer()
{

}

Window* Renderer::ReturnWindow()
{
	return TheWindow;
}


