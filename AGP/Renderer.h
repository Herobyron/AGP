#pragma once
#include "Window.h"

class Renderer
{
private:

	Window* TheWindow;
	
	//vertext buffer desc (for set up)
	D3D11_BUFFER_DESC BufferDesc;

	//constant buffer
	D3D11_BUFFER_DESC ConstantBufferDesc;

	//Vertex buffer
	ID3D11Buffer* VertexBuffer;

	//Sampler Desc
	D3D11_SAMPLER_DESC SamplerDesc;


public:

	Renderer();
	Renderer(HINSTANCE hInstance, int CommandShow);
	~Renderer();

	Window* ReturnWindow();
	
	HRESULT InitialseGraphics();
	
	void RenderUpdate();

};

