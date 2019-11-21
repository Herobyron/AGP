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

// come back to this later as im not sure what actually needs to be in this
HRESULT Renderer::InitialseGraphics()
{
	HRESULT hr = S_OK;

	// set up and create vertex buffer
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	

	return S_OK;
}

void Renderer::RenderUpdate()
{

}

