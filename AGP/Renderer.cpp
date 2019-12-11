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

}

Renderer::Renderer(DirectXSetUp* DirectXRef)
{
	DeviceRef = DirectXRef->ReturnDevice();
	ImmediateContextRef = DirectXRef->ReturnImmediateContext();
	SwapchainRef = DirectXRef->ReturnSwapChain();

	ConstantBuffer = NULL;
	VertexBuffer = NULL;
	Sampler = NULL;
	Vertexshader = NULL;
	PixelShader = NULL;

	//intialise everything
	InitialseGraphics();
}


Renderer::~Renderer()
{
	if (TestModel) delete TestModel;

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




	
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = (36 * 36);
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = DirectX->ReturnDevice()->CreateBuffer(&BufferDesc, NULL, &VertexBuffer);

	if (FAILED(hr))
	{
		return hr;
	}


	ZeroMemory(&ConstantBufferDesc, sizeof(ConstantBufferDesc));
	ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ConstantBufferDesc.ByteWidth = 112;
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = DirectX->ReturnDevice()->CreateBuffer(&ConstantBufferDesc, NULL, &ConstantBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	DirectX->ReturnImmediateContext()->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MS);

	memcpy(MS.pData, TestModel->vertices, sizeof(36 * 36));

	DirectX->ReturnImmediateContext()->Unmap(VertexBuffer, NULL);




	return S_OK;
}

void Renderer::RenderUpdate(Camera* camera)
{
	DirectX->ReturnImmediateContext()->ClearRenderTargetView(DirectX->ReturnbufferRTView(), m_ClearColour);
	DirectX->ReturnImmediateContext()->ClearDepthStencilView(DirectX->ReturnZBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//put the object file model stuff here


	DirectX->ReturnImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0), 640.0 / 480, 1.0, 100);
	
	view = camera->GetViewMatrix();

	TestModel->SetXPos(camera->GetX());
	TestModel->SetYPos(camera->GetY());
	TestModel->SetZPos(camera->GetZ());

	TestModel->Draw(&view, &projection);

	DirectX->ReturnSwapChain()->Present(0, 0);
}



DirectX::XMMATRIX Renderer::ReturnProjection()
{
	return projection;
}
