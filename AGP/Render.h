#pragma once
#include "DirectSetUp.h"
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include <directxmath.h>

//define the vertex structure
struct POS_COL_VERTEX
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 Col;
};

//defining the constant buffer structure
struct CONSTANT_BUFFER0
{
	DirectX::XMMATRIX WorldViewProjection;	// 64 bytes
	float RedAmount;						// 4 bytes
	float Scale;							// 4 bytes
	DirectX::XMFLOAT2 packing_bytes;		// 2x4 bytes = 12 bytes
};

class Render
{
private:

	ID3D11Buffer* VertexBuffer;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;

	ID3D11Buffer* ConstantBuffer0;

public:

	Render();
	~Render();


	HRESULT IntialiseGraphics(DirectSetUp* SetUp);
	void RenderFrame(DirectSetUp* SetUp);


};

