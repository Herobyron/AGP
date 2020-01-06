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

	// the direct x set up constructor
	DirectSetUp();

	// the direct x set up destructor
	~DirectSetUp();

	// initialise the direct x components
	HRESULT InitialseD3D(Window* window);

	// returns the immediate context of the project
	ID3D11DeviceContext* ReturnImmediateContext();
	
	// returns the device thats been created from the project
	ID3D11Device* ReturnDevice();
	
	// returns the swap chain created from the project
	IDXGISwapChain* ReturnSwapChain();

	//returns the back buffer view created by initialise D3D
	ID3D11RenderTargetView* ReturnBackBufferView();

	//returns the Z buffer created from initialise D3D
	ID3D11DepthStencilView* ReturnZBuffer();

	// returns the text which is initialised in this class
	Text2D* ReturnText();

};

