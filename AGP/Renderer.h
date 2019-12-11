#pragma once
#include "Model.h"
#include "DirectXSetUp.h"
#include "Camera.h"


class Renderer
{
private:

	// a test model for when i want to test wether things will render
	Model* TestModel;

	//vertext buffer desc (for set up)
	D3D11_BUFFER_DESC BufferDesc;

	//constant buffer Desc used within Initialise Grpahics to create constant buffer
	D3D11_BUFFER_DESC ConstantBufferDesc;

	//constant buffer initialised within IntilaiseGrpahics()
	ID3D11Buffer* ConstantBuffer;

	//Vertex buffer initialised within IntialiseGraphics()
	ID3D11Buffer* VertexBuffer;

	//Sampler Desc used to make the Sampler
	D3D11_SAMPLER_DESC SamplerDesc;

	//The Sampler declared here and intitialsed within Intilaisegrpahics
	ID3D11SamplerState* Sampler;

	//need to create the input class
	//input latyout declare here

	//The Device Which is created within DirectXSetup and passed through the Renderer Constructor when made
	ID3D11Device* DeviceRef;

	// the immediate contecxt which is created within the DirectxSetup and passd throguh the Renderer Constructor when made
	ID3D11DeviceContext* ImmediateContextRef;

	//the Swap chain which is created within DirectXSetup and passed through the Renderer Constructor when made
	IDXGISwapChain* SwapchainRef;

	//A Mapped Subresource
	D3D11_MAPPED_SUBRESOURCE MS;

	// the Vertex Shader. which is intialised within Intialise Graphics
	ID3D11VertexShader* Vertexshader;

	//the Pixel Shader. which is intialised within Intialise Graphics
	ID3D11PixelShader* PixelShader;


	// the colour needed to clear the screen
	float m_ClearColour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };

	//view and projection matrixes calculated with the camera
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX view;


public:

	//the standard contructor which will set all pointers to null
	Renderer();

	// a Second Contstructor which Takes a refernce to the DirectX class
	// this is used to get a refernce to the Device, ImmediateContext, and swapchain
	Renderer(DirectXSetUp* DirectXRef);

	// the deconstrucotr to make sure that all the pointers are destroyed within order
	~Renderer();

	// intialise graphics creates and stores a low of the buffers and shaders so they are ready to be draw
	HRESULT InitialseGraphics();
	
	//used to return the projection matric
	DirectX::XMMATRIX ReturnProjection();

	// render update changes everythings position and renders everything in view
	void RenderUpdate(Camera* camera);

};

