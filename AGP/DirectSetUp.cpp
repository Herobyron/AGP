#include "DirectSetUp.h"
// the direct x base constructor
DirectSetUp::DirectSetUp()
{

}

//direct x destructor
DirectSetUp::~DirectSetUp()
{
	if (Device) Device->Release();
	if (ImmediateContext) ImmediateContext->Release();
	if (SwapChain) SwapChain->Release();
	if (ZBuffer) ZBuffer->Release();
	if (The2DText) delete The2DText;
}


// the direct x initialise function
HRESULT DirectSetUp::InitialseD3D(Window* window)
{
	HRESULT hr = S_OK;

	//creates the rectanle 
	RECT rc;
	GetClientRect(window->ReturnHWND(), &rc);
	UINT width = rc.right - rc.left;
	UINT hieght = rc.bottom - rc.top;

	UINT CreateDeviceFlags = 0;

	//sets the driver types to use
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//sets the feature levels to use
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//setting up the swap chain
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

	//creating the swap chain
	 for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	 {
		 DriverType = driverTypes[driverTypeIndex];
		 hr = D3D11CreateDeviceAndSwapChain(NULL, DriverType, NULL, CreateDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &FeatureLevel, &ImmediateContext);
		 if (SUCCEEDED(hr))
			 break;
	 
	 
	 }

	 //if failed end
	 if (FAILED(hr))
		 return hr;

	 //get a pointer to the back buffer texture
	 hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& BackbufferTexture);

	 if (FAILED(hr)) return hr;

	 //use the back buffer texture to create the render target view
	 hr = Device->CreateRenderTargetView(BackbufferTexture, NULL, &BackBufferRtView);

	 BackbufferTexture->Release();

	 if (FAILED(hr)) return hr;

	 //creating the z buffer texture
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

	 ID3D11Texture2D* ZBufferTexture;
	 hr = Device->CreateTexture2D(&tex2dDesc, NULL, &ZBufferTexture);

	 if (FAILED(hr))
	 {
		 return hr;
	 }

	 //create the z buffer
	 D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	 ZeroMemory(&DSVDesc, sizeof(DSVDesc));

	 DSVDesc.Format = tex2dDesc.Format;
	 DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	 Device->CreateDepthStencilView(ZBufferTexture, &DSVDesc, &ZBuffer);
	 ZBufferTexture->Release();

	 //set the redner target view
	 ImmediateContext->OMSetRenderTargets(1, &BackBufferRtView, ZBuffer);

	 //set the viewport
	 D3D11_VIEWPORT viewport;

	 viewport.TopLeftX = 0;
	 viewport.TopLeftY = 0;
	 viewport.Width = (FLOAT)width;
	 viewport.Height = (FLOAT)hieght;
	 viewport.MinDepth = 0.0f;
	 viewport.MaxDepth = 1.0f;

	 ImmediateContext->RSSetViewports(1, &viewport);

	 //text stuff
	 The2DText = new Text2D("assets/font1.bmp", Device, ImmediateContext);

	 return S_OK;

}

// a function to return the device
ID3D11Device* DirectSetUp::ReturnDevice()
{
	return Device;
}

//returns the immediatecontext
ID3D11DeviceContext* DirectSetUp::ReturnImmediateContext()
{
	return ImmediateContext;
}

//returns the swap chain
IDXGISwapChain* DirectSetUp::ReturnSwapChain()
{
	return SwapChain;
}

// returns the back buffer view
ID3D11RenderTargetView* DirectSetUp::ReturnBackBufferView()
{
	return BackBufferRtView;
}

//returns the z buffer
ID3D11DepthStencilView* DirectSetUp::ReturnZBuffer()
{
	return ZBuffer;
}

//returns the text
Text2D* DirectSetUp::ReturnText()
{
	return The2DText;
}