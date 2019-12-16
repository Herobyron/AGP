#include "Renderer.h"


Renderer::Renderer()
{
	ConstantBuffer = NULL;
	VertexBuffer = NULL;
	Sampler = NULL;
	DeviceRef = NULL;
	ImmediateContextRef = NULL;
	SwapchainRef = NULL;
	Vertexshader = NULL;
	PixelShader = NULL;
	BackBufferRTViewRef = NULL;
	ZBufferRef = NULL;
	Inputs = new Input();
	TestShape = new Shape();
}

Renderer::Renderer(DirectXSetUp* DirectXRef, Input* theinputs)
{
	DeviceRef = DirectXRef->ReturnDevice();
	ImmediateContextRef = DirectXRef->ReturnImmediateContext();
	SwapchainRef = DirectXRef->ReturnSwapChain();
	BackBufferRTViewRef = DirectXRef->ReturnbufferRTView();
	ZBufferRef = DirectXRef->ReturnZBuffer();

	//ConstantBuffer = NULL;
	//VertexBuffer = NULL;
	//Sampler = NULL;
	//Vertexshader = NULL;
	//PixelShader = NULL;

	//create the test shape
	TestShape = new Shape();

	//create the inputs
	Inputs = theinputs;

	//intialise everything
	InitialseGraphics();
}


Renderer::~Renderer()
{
	if (TestModel)  delete TestModel;
	if (Inputs)		delete Inputs;
	if (TestShape)  delete TestShape;

	if (VertexBuffer) VertexBuffer->Release();
	if (Vertexshader) Vertexshader->Release();
	if (PixelShader) PixelShader->Release();
	if (SwapchainRef) SwapchainRef->Release();
	if (ConstantBuffer) ConstantBuffer->Release();
	if (Sampler) Sampler->Release();
	if (ImmediateContextRef) ImmediateContextRef->Release();
	if (DeviceRef) DeviceRef->Release();
	
}

// come back to this later as im not sure what actually needs to be in this
HRESULT Renderer::InitialseGraphics()
{
	HRESULT hr = S_OK;

	//loading in a Model (Testing)
	//TestModel = new Model(DirectX->ReturnDevice(), DirectX->ReturnImmediateContext());
	//TestModel->LoadObjModel((char*) "assets/Sphere.obj");
	//
	//TestModel->SetScale(0.1);
	//TestModel->SetXAngle(0);
	//TestModel->SetXPos(0);
	//TestModel->SetYPos(0);
	//TestModel->SetZPos(0);

	

	//set up and create the vertex buffer
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(POS_COL_TEX_NORM_VERTEX) * 36;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//creating the buffuer
	hr = DeviceRef->CreateBuffer(&BufferDesc, NULL, &VertexBuffer);

	//check to see if it has failed
	if (FAILED(hr))
	{
		return hr;
	}

	//setting up and creating the constant buffer
	ZeroMemory(&ConstantBufferDesc, sizeof(ConstantBufferDesc));
	ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstantBufferDesc.ByteWidth = 112;
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = DeviceRef->CreateBuffer(&ConstantBufferDesc, NULL, &ConstantBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	ImmediateContextRef->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MS);

	memcpy(MS.pData, TestShape->vertices, sizeof(36 * 36));

	ImmediateContextRef->Unmap(VertexBuffer, NULL);

	//load and compile the vertex and pixel shaders
	ID3DBlob* VS, * PS, * Error;

	D3DX11CreateShaderResourceViewFromFile(DeviceRef, "assets/texture.png", NULL, NULL, &Texture, NULL);

	//Note Error is bieng thrown from the shader wonder what it is
	hr = D3DX11CompileFromFile("Shaders.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &VS, &Error, 0);

	//checks to see if shader was compiled correctly
	if (Error != 0)
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	hr = D3DX11CompileFromFile("Shaders.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &PS, &Error, 0);

	//checks to see if the shader was compiled without errors
	if (Error != 0)
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	//create the shader object (Vertex shader)
	hr = DeviceRef->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &Vertexshader);
	
	//if setup failed exit
	if (FAILED(hr))
	{
		return hr;
	}

	//create the shader object (pixel shader)
	hr = DeviceRef->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);

	//if setup failed exit
	if (FAILED(hr))
	{
		return hr;
	}

	//set the shader objects as active
	ImmediateContextRef->VSSetShader(Vertexshader, 0, 0);
	ImmediateContextRef->PSSetShader(PixelShader, 0, 0);

	//create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//creating the input layout
	hr = DeviceRef->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);

	// check to see if it has failed
	if (FAILED(hr))
	{
		return hr;
	}

	//setup the layout
	ImmediateContextRef->IASetInputLayout(InputLayout);

	return S_OK;
}

void Renderer::RenderUpdate(Camera* camera)
{

	ImmediateContextRef->VSSetShader(Vertexshader, 0, 0);
	ImmediateContextRef->PSSetShader(PixelShader, 0, 0);
	ImmediateContextRef->IASetInputLayout(InputLayout);

	ImmediateContextRef->ClearRenderTargetView(BackBufferRTViewRef, m_ClearColour);
	ImmediateContextRef->ClearDepthStencilView(ZBufferRef, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	UINT stride = sizeof(POS_COL_TEX_NORM_VERTEX);
	UINT offset = 0;
	ImmediateContextRef->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//select which primative type to use
	ImmediateContextRef->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ImmediateContextRef->PSSetSamplers(0, 1, &Sampler);
	ImmediateContextRef->PSGetShaderResources(0, 1, &Texture);


	//calculations stuff

	ImmediateContextRef->UpdateSubresource(ConstantBuffer, 0, 0, &CB0Values, 0, 0);
	ImmediateContextRef->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	ImmediateContextRef->PSSetConstantBuffers(0, 1, &ConstantBuffer);
	ImmediateContextRef->Draw(36, 0);

	//projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0), 640.0 / 480, 1.0, 100);
	//
	//view = camera->GetViewMatrix();
	//
	//TestModel->SetXPos(camera->GetX());
	//TestModel->SetYPos(camera->GetY());
	//TestModel->SetZPos(camera->GetZ());
	//
	//TestModel->Draw(&view, &projection);

	SwapchainRef->Present(0, 0);
}



DirectX::XMMATRIX Renderer::ReturnProjection()
{
	return projection;
}
