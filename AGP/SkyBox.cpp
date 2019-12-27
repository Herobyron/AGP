#include "SkyBox.h"

//the constructor
SkyBox::SkyBox(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{

	SkyBoxDevice = device;
	SkyBoxImmediateContext = immediatecontext;
	
	SkyBoxObject = new Model(SkyBoxDevice, SkyBoxImmediateContext);
	
	SkyBoxObject->SetX(0);
	SkyBoxObject->SetY(0);
	SkyBoxObject->SetZ(0);
	SkyBoxObject->SetXAngle(0);
	SkyBoxObject->SetYAngle(0);
	SkyBoxObject->SetZAngle(0);
	SkyBoxObject->SetScale(3.0f);
}

SkyBox::~SkyBox()
{
	if (SkyBoxDevice) SkyBoxDevice->Release();
	if (SkyBoxImmediateContext) SkyBoxImmediateContext->Release();
	if (SkyBoxObject) delete SkyBoxObject;
	if (SkyBoxVShader) SkyBoxVShader->Release();
	if (SkyBoxPShader) SkyBoxPShader->Release();
	if (SkyBoxInputLayout) SkyBoxInputLayout->Release();
	if (SkyBoxConstantBuffer) SkyBoxConstantBuffer->Release();
	if (RasterSkyBox) RasterSkyBox->Release();
	if (RasterSolid) RasterSolid->Release();
	if (DepthWriteSolid) DepthWriteSolid->Release();
	if (DepthWriteSkyBox) DepthWriteSkyBox->Release();

	if (SkyBoxSampler) SkyBoxSampler->Release();
	if (SkyBoxTexture) SkyBoxTexture->Release();
}

HRESULT SkyBox::SkyBoxInitialisation()
{
	HRESULT hr = S_OK;


	SKYVout vertices[] =
	{
	
		//back face
		{DirectX::XMFLOAT3(-1.0f,  1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
	
		//Front Face
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
	
		//left face
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
	
	
		//right face
		{DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
	
	
		//bottom face
		{DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
	
	
		//top face
		{DirectX::XMFLOAT3( 1.0f, 1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, 1.0f,  1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)}
	
	
	
	};

	//trying the model way
	
	SkyBoxObject->LoadObjModel((char*)"assets/cube.obj");

	//setting up the skybox stuff
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	hr = SkyBoxDevice->CreateRasterizerState(&rasterizer_desc, &RasterSolid);

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	hr = SkyBoxDevice->CreateRasterizerState(&rasterizer_desc, &RasterSkyBox);

	D3D11_DEPTH_STENCIL_DESC DSDesc;
	ZeroMemory(&DSDesc, sizeof(DSDesc));
	DSDesc.DepthEnable = true;
	DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSDesc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = SkyBoxDevice->CreateDepthStencilState(&DSDesc, &DepthWriteSolid);
	
	DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = SkyBoxDevice->CreateDepthStencilState(&DSDesc, &DepthWriteSkyBox);

	D3DX11CreateShaderResourceViewFromFile(SkyBoxDevice, "assets/skybox02.dds", NULL, NULL, &SkyBoxTexture, NULL);

	//loading in the vertex and pixel shaders of the model
	ID3DBlob* VS1, * PS1, * Error1;
	
	//finding the vertex shader
	hr = D3DX11CompileFromFile("SkyShader.hlsl", 0, 0, "SkyVS", "vs_4_0", 0, 0, 0, &VS1, &Error1, 0);
	
	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}
	
	//finding the pixel shader
	hr = D3DX11CompileFromFile("SkyShader.hlsl", 0, 0, "SkyPS", "ps_4_0", 0, 0, 0, &PS1, &Error1, 0);
	
	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}
	
	//create the shader objects
	hr = SkyBoxDevice->CreateVertexShader(VS1->GetBufferPointer(), VS1->GetBufferSize(), NULL, &SkyBoxVShader);
	
	if (FAILED(hr))
		return hr;
	
	hr = SkyBoxDevice->CreatePixelShader(PS1->GetBufferPointer(), PS1->GetBufferSize(), NULL, &SkyBoxPShader);
	
	if (FAILED(hr))
		return hr;
	
	
	//set up the shader objects as active
	SkyBoxImmediateContext->VSSetShader(SkyBoxVShader, 0, 0);
	SkyBoxImmediateContext->PSSetShader(SkyBoxPShader, 0, 0);
	
	//create and set the input layout
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	hr = SkyBoxDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS1->GetBufferPointer(), VS1->GetBufferSize(), &SkyBoxInputLayout);
	
	if (FAILED(hr))
		return hr;
	
	SkyBoxImmediateContext->IASetInputLayout(SkyBoxInputLayout);
	
	//set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//creating the buffer
	hr = SkyBoxDevice->CreateBuffer(&bufferDesc, NULL, &SkyBoxVertexBuffer);
	
	//cehck to see if failed and return if it did
	if (FAILED(hr))
	{
		return hr;
	}


	//constant buffer for model
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = 64;
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	
	hr = SkyBoxDevice->CreateBuffer(&constant_buffer_desc, NULL, &SkyBoxConstantBuffer);
	
	if (FAILED(hr))
	{
		return hr;
	}

	//copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	
	//lock the buffer to allow for writing
	SkyBoxImmediateContext->Map(SkyBoxVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	
	//copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));
	
	//unlock the buffer
	SkyBoxImmediateContext->Unmap(SkyBoxVertexBuffer, NULL);


	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	SkyBoxDevice->CreateSamplerState(&sampler_desc, &SkyBoxSampler);

	SkyBoxImmediateContext->PSSetSamplers(0, 1, &SkyBoxSampler);
	SkyBoxImmediateContext->PSSetShaderResources(0, 1, &SkyBoxTexture);

	return hr;
}

void SkyBox::Draw(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	SkyBox_CONSTANT_BUFFER MCB_Values;
	DirectX::XMMATRIX world, scale, rotate, translate;

	scale = DirectX::XMMatrixScaling(SkyBoxScale, SkyBoxScale, SkyBoxScale);
	rotate = DirectX::XMMatrixRotationRollPitchYaw(SkyBoxAngleX, SkyBoxAngleY, SkyBoxAngleZ);
	translate = DirectX::XMMatrixTranslation(SkyBoxX, SkyBoxY, SkyBoxZ);

	world = scale * rotate * translate;

	MCB_Values.WorldViewProjection = world * (view) * (projection);

	SkyBoxImmediateContext->VSSetConstantBuffers(0, 1, &SkyBoxConstantBuffer);
	SkyBoxImmediateContext->PSSetConstantBuffers(0, 1, &SkyBoxConstantBuffer);

	SkyBoxImmediateContext->UpdateSubresource(SkyBoxConstantBuffer, 0, 0, &ConstantBufferValues, 0, 0);

	SkyBoxImmediateContext->RSSetState(RasterSkyBox);
	SkyBoxImmediateContext->OMSetDepthStencilState(DepthWriteSkyBox, 0);

	SkyBoxObject->Draw(view, projection);
	//SkyBoxImmediateContext->Draw(36, 0);
	SkyBoxImmediateContext->RSSetState(RasterSolid);
	SkyBoxImmediateContext->OMSetDepthStencilState(DepthWriteSolid, 0);

}

void SkyBox::SetBoxX(float X)
{
	SkyBoxObject->SetX(X);
}

void SkyBox::SetBoxY(float Y)
{
	SkyBoxObject->SetY(Y);
}

void SkyBox::SetBoxZ(float Z)
{
	SkyBoxObject->SetZ(Z);
}

void SkyBox::SetBoxXAngle(float x)
{
	SkyBoxObject->SetXAngle(x);
}


void SkyBox::SetBoxYAngle(float y)
{
	SkyBoxObject->SetYAngle(y);
}

void SkyBox::SetBoxZAngle(float z)
{
	SkyBoxObject->SetZAngle(z);
}

void SkyBox::SetBoxScale(float scale)
{
	SkyBoxObject->SetScale(scale);
}

float SkyBox::ReturnXPos()
{
	return SkyBoxX;
}

float SkyBox::ReturnYPos()
{
	return SkyBoxY;
}

float SkyBox::ReturnZPos()
{
	return SkyBoxZ;
}

float SkyBox::ReturnXAngle()
{
	return SkyBoxAngleX;
}

float SkyBox::ReturnYAngle()
{
	return SkyBoxAngleY;
}

float SkyBox::ReturnZAngle()
{
	return SkyBoxAngleZ;
}

float SkyBox::ReturnScale()
{
	return SkyBoxScale;
}