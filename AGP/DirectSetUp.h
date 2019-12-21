#pragma once
#include "Window.h"
#include "text2D.h"

class DirectSetUp
{
private:
	
	//a pointer to the back buffer texture
	ID3D11RenderTargetView* BackBufferRtView = NULL;

	// pointer to the back buffer texture
	ID3D11Texture2D* BackbufferTexture;

	//driver types and feature level
	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	//the device
	ID3D11Device* Device = NULL;

	//the immediate context
	ID3D11DeviceContext* ImmediateContext = NULL;

	//the swapchain
	IDXGISwapChain* SwapChain = NULL;

	//the Z buffer
	ID3D11DepthStencilView* ZBuffer;

	// a 2d text pointer
	Text2D * The2DText;

public:

	DirectSetUp();
	~DirectSetUp();

	HRESULT InitialseD3D(Window* window);

	ID3D11DeviceContext* ReturnImmediateContext();
	ID3D11Device* ReturnDevice();
	IDXGISwapChain* ReturnSwapChain();

	ID3D11RenderTargetView* ReturnBackBufferView();

	ID3D11DepthStencilView* ReturnZBuffer();

	Text2D* ReturnText();

};

