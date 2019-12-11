#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>
#include <windowsx.h>

#include "Window.h"

class DirectXSetUp
{
private:

	//A Rect which is used within IntialiseD3D()
	RECT rc;

	//the HWND which is initialised in constructor. got from window
	HWND Hwnd;

	// the Device refernce. which is declared here and intialised within InitialiseD3D()
	ID3D11Device* Device = nullptr;
	
	//immediate context refernce. which is declared here and intialised within InitialiseD3D()
	ID3D11DeviceContext* ImmediateContext = nullptr;

	// swap chain which is declared here and initialised within InitialiseD3D()
	IDXGISwapChain* SwapChain = nullptr;

	//the Z Buffer which is declared here and intialised within IntialiseD3D()
	ID3D11DepthStencilView* ZBuffer = nullptr;

	//the Z buffer Texture is declared here and initialsed within InitialiseD3D()
	ID3D11Texture2D* ZBufferTexture = nullptr;

	//The BackBuffer pointer. which is declared here and initialised within InitialiseD3D()
	ID3D11RenderTargetView* BackBufferRTView = nullptr;
	
	//the backbuffer Texture. which is declared here and initialsied within InitialiseD3D()
	ID3D11Texture2D* BackBufferTexture;

	// the driver type and feature level both declared within directX class. within InitialiseD3D()
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

public:

	DirectXSetUp(Window* Window);
	~DirectXSetUp();

	HRESULT InitialiseD3D();
	ID3D11Device* ReturnDevice();
	ID3D11DeviceContext* ReturnImmediateContext();

	ID3D11DepthStencilView* ReturnZBuffer();
	ID3D11RenderTargetView* ReturnbufferRTView();

	IDXGISwapChain* ReturnSwapChain();

};

