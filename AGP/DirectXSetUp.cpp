#include "DirectXSetUp.h"

DirectXSetUp::DirectXSetUp(Window* Window)
{
	Device = NULL;
	ImmediateContext = NULL;
	SwapChain = NULL;

	ZBuffer = nullptr;
	ZBufferTexture = nullptr;
	BackBufferRTView = NULL;

	Hwnd = Window->GetHWND();
	
	//intialise everything
	InitialiseD3D();
}

DirectXSetUp::~DirectXSetUp()
{

}

HRESULT DirectXSetUp::InitialiseD3D()
{
	HRESULT hr = S_OK;

	GetClientRect(Hwnd, &rc);
	UINT width  = rc.right  - rc.left;
	UINT hieght = rc.bottom - rc.top;
	
	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, //comment out this is trying to test D3D 11 on stuff that doesnt support it 
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = hieght;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &SwapChain,
			&Device, &g_featureLevel, &ImmediateContext);
		if (SUCCEEDED(hr))
			break;

	}

	if (FAILED(hr))
		return hr;

	//Get pointer to back buffer texture
	BackBufferTexture;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& BackBufferTexture);

	if (FAILED(hr)) return hr;

	//use the back buffer texture pointer to create the render target view
	hr = Device->CreateRenderTargetView(BackBufferTexture, NULL, &BackBufferRTView);
	BackBufferTexture->Release();

	if (FAILED(hr)) return hr;

	//create a Z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = hieght;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ZBufferTexture;
	hr = Device->CreateTexture2D(&tex2dDesc, NULL, &ZBufferTexture);

	if (FAILED(hr)) return hr;

	//create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	Device->CreateDepthStencilView(ZBufferTexture, &dsvDesc, &ZBuffer);
	ZBufferTexture->Release();

	//set the render target view
	ImmediateContext->OMSetRenderTargets(1, &BackBufferRTView, ZBuffer);

	//set the viewport
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)hieght;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	ImmediateContext->RSSetViewports(1, &viewport);

	
	return S_OK;

}

ID3D11Device* DirectXSetUp::ReturnDevice()
{
	return Device;
}

ID3D11DeviceContext* DirectXSetUp::ReturnImmediateContext()
{
	return ImmediateContext;
}

ID3D11DepthStencilView* DirectXSetUp::ReturnZBuffer()
{
	return ZBuffer;
}

ID3D11RenderTargetView* DirectXSetUp::ReturnbufferRTView()
{
	return BackBufferRTView;
}

IDXGISwapChain* DirectXSetUp::ReturnSwapChain()
{
	return SwapChain;
}

