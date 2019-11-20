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

	ID3D11Device* Device;
	ID3D11DeviceContext* ImmediateContext;
	IDXGISwapChain* SwapChain;

	//change these names
	ID3D11DepthStencilView* g_pZBuffer;
	ID3D11RenderTargetView* g_pBackBufferRTView = NULL;


	Window* TheWindow;

	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

public:

	DirectXSetUp(Window* Window);
	~DirectXSetUp();

	HRESULT InitialiseD3D();



};

