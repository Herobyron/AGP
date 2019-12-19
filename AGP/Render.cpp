#include "Render.h"

//
Render::Render()
{

}


//
Render::~Render()
{
	if (VertexBuffer) VertexBuffer->Release();
	if (InputLayout) InputLayout->Release();
	if (VertexShader) VertexShader->Release();
	if (PixelShader) PixelShader->Release();
	if (ConstantBuffer0) ConstantBuffer0->Release();
}


//the render frame
void Render::RenderFrame(DirectSetUp* SetUp)
{
	//updating the constant buffer
	CONSTANT_BUFFER0 CB0_Values;
	CB0_Values.RedAmount = 0.0f; //50% of the vertex red value
	CB0_Values.Scale = 0.5f; //50% of the vertex scale

	//updating the world view projection matrix
	DirectX::XMMATRIX projection, world, view;

	world *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(180));
	world = DirectX::XMMatrixTranslation(0, 0, 5);


	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0), 640.0 / 480.0, 1.0, 100.0);
	view = DirectX::XMMatrixIdentity();
	CB0_Values.WorldViewProjection = world * view * projection;


	//clear the back buffer 
	float rgba_clear_color[4] = { 0.3f, 0.3f, 0.7f, 1.0f };
	SetUp->ReturnImmediateContext()->ClearRenderTargetView(SetUp->ReturnBackBufferView(), rgba_clear_color);

	//render here
	//set the vertex buffer
	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	SetUp->ReturnImmediateContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	
	//selectwhich primtive to use
	SetUp->ReturnImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//upload the new values for the constant buffer
	SetUp->ReturnImmediateContext()->UpdateSubresource(ConstantBuffer0, 0, 0, &CB0_Values, 0, 0);

	//seting the constant buffer to be active
	SetUp->ReturnImmediateContext()->VSSetConstantBuffers(0, 1, &ConstantBuffer0);

	//draw the vertex buffer to the back buffer
	SetUp->ReturnImmediateContext()->Draw(3, 0);


	//display what was rendered
	SetUp->ReturnSwapChain()->Present(0, 0);

}

HRESULT Render::IntialiseGraphics(DirectSetUp* SetUp)
{
	HRESULT hr = S_OK;

	//define verticies of a triangle - screen coordinates -1 to +1
	POS_COL_VERTEX verticies[] =
	{
		{DirectX::XMFLOAT3(0.9f, 0.9f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(0.9f, -0.9f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-0.9f, -0.9f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	//set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(POS_COL_VERTEX) * 3;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//creating the buffer
	hr = SetUp->ReturnDevice()->CreateBuffer(&bufferDesc, NULL, &VertexBuffer);

	//cehck to see if failed and return if it did
	if (FAILED(hr))
	{
		return hr;
	}

	//setting up and creating the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //can be updated with update subresource
	constant_buffer_desc.ByteWidth = 80; // must be a multiple of 16. this number is calculated from the constant buffer structure
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //use as a constant

	hr = SetUp->ReturnDevice()->CreateBuffer(&constant_buffer_desc, NULL, &ConstantBuffer0);

	if (FAILED(hr))
	{
		return hr;
	}



	//copy the bertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//lock the buffer to allow for writing
	SetUp->ReturnImmediateContext()->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//copy the data
	memcpy(ms.pData, verticies, sizeof(verticies));

	//unlock the buffer
	SetUp->ReturnImmediateContext()->Unmap(VertexBuffer, NULL);

	//load and compile the pixel and vertex shader (using vs_5_0 to target DX11 hardware only)
	ID3DBlob* VS, *PS, *Error;

	//finding the vertex shader
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &Error, 0);

	if (Error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
			return hr;
	}

	//finding the pixel shader
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &Error, 0);

	if (Error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
			return hr;
	}


	//create the shader objects
	hr = SetUp->ReturnDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VertexShader);

	if (FAILED(hr))
		return hr;

	hr = SetUp->ReturnDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);

	if (FAILED(hr))
		return hr;

	//set the shader objects as active
	SetUp->ReturnImmediateContext()->VSSetShader(VertexShader, 0, 0);
	SetUp->ReturnImmediateContext()->PSSetShader(PixelShader, 0, 0);

	//create and set the input layout
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//make sure they are correct dxgi format and d3d versions
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};

	hr = SetUp->ReturnDevice()->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);

	if (FAILED(hr))
		return hr;

	SetUp->ReturnImmediateContext()->IASetInputLayout(InputLayout);

	return S_OK;

}
