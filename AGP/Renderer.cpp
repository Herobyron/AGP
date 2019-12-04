#include "Renderer.h"


Renderer::Renderer()
{

}

Renderer::Renderer(HINSTANCE hInstance, int CommandShow)
{
	TheWindow = new Window(hInstance, CommandShow);
	DirectX = new DirectXSetUp(ReturnWindow());



}


Renderer::~Renderer()
{
	if (TheWindow) delete TheWindow;
	if (TestModel) delete TestModel;
	if (DirectX)   delete DirectX;
}

Window* Renderer::ReturnWindow()
{
	return TheWindow;
}

// come back to this later as im not sure what actually needs to be in this
HRESULT Renderer::InitialseGraphics()
{
	HRESULT hr = S_OK;

	//loading in a Model (Testing)
	TestModel = new Model(DirectX->ReturnDevice(), DirectX->ReturnImmediateContext());
	TestModel->LoadObjModel((char*) "assets/Sphere.obj");

	TestModel->SetScale(1);
	TestModel->SetXAngle(0);
	//TestModel->SetXPos(0);
	//TestModel->SetYPos(0);
	//TestModel->SetZPos(0);


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


DirectXSetUp* Renderer::ReturnDirectX()
{
	return DirectX;
}

DirectX::XMMATRIX Renderer::ReturnProjection()
{
	return projection;
}
