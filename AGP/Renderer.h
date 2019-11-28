#pragma once
#include "Window.h"
#include "Model.h"
#include "DirectXSetUp.h"


class Renderer
{
private:

	Window* TheWindow;
	Model* TestModel;
	DirectXSetUp* DirectX;

	//vertext buffer desc (for set up)
	D3D11_BUFFER_DESC BufferDesc;

	//constant buffer
	D3D11_BUFFER_DESC ConstantBufferDesc;

	//Vertex buffer
	ID3D11Buffer* VertexBuffer;

	//Sampler Desc
	D3D11_SAMPLER_DESC SamplerDesc;

	//stuff i know is needed
	//The BackBuffer
	ID3D11RenderTargetView* m_BackBufferRTView = nullptr;
	ID3D11Texture2D* m_BackBufferTexture = nullptr;

	// the ZBuffer
	ID3D11DepthStencilView* m_ZBuffer = nullptr;

	float m_ClearColour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX view;



public:

	Renderer();
	Renderer(HINSTANCE hInstance, int CommandShow);
	~Renderer();

	Window* ReturnWindow();
	
	HRESULT InitialseGraphics();
	
	void RenderUpdate();

};

