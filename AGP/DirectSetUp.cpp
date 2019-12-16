#include "DirectSetUp.h"
DirectSetUp::DirectSetUp()
{

}

DirectSetUp::~DirectSetUp()
{
	if (Device) Device->Release();
	if (ImmediateContext) ImmediateContext->Release();
	if (SwapChain) SwapChain->Release();
}



HRESULT DirectSetUp::InitialseD3D(Window* window)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(window->ReturnHWND(), &rc);
	UINT width = rc.right - rc.left;
	UINT hieght = rc.bottom - rc.top;

	UINT CreateDeviceFlags = 0;


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
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
	sd.OutputWindow = window->ReturnHWND();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	 for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	 {
		 DriverType = driverTypes[driverTypeIndex];
		 hr = D3D11CreateDeviceAndSwapChain(NULL, DriverType, NULL, CreateDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &FeatureLevel, &ImmediateContext);
		 if (SUCCEEDED(hr))
			 break;
	 
	 
	 }

	 if (FAILED(hr))
		 return hr;

	 //get a pointer to the back buffer texture
	 hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& BackbufferTexture);

	 if (FAILED(hr)) return hr;

	 //use the back buffer texture to create the render target view
	 hr = Device->CreateRenderTargetView(BackbufferTexture, NULL, &BackBufferRtView);

	 BackbufferTexture->Release();

	 if (FAILED(hr)) return hr;

	 //set the redner target view
	 ImmediateContext->OMSetRenderTargets(1, &BackBufferRtView, NULL);

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


ID3D11Device* DirectSetUp::ReturnDevice()
{
	return Device;
}

ID3D11DeviceContext* DirectSetUp::ReturnImmediateContext()
{
	return ImmediateContext;
}

IDXGISwapChain* DirectSetUp::ReturnSwapChain()
{
	return SwapChain;
}

ID3D11RenderTargetView* DirectSetUp::ReturnBackBufferView()
{
	return BackBufferRtView;
}