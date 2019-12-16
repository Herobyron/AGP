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

class Render
{
private:

	ID3D11Buffer* VertexBuffer;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;

public:

	Render();
	~Render();


	HRESULT IntialiseGraphics(DirectSetUp* SetUp);
	void RenderFrame(DirectSetUp* SetUp);


};

