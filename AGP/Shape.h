#pragma once
#include <DirectXMath.h>

struct POS_COL_TEX_NORM_VERTEX //this willbe added to and renamed in future tutorials
{
	DirectX::XMFLOAT3 vertex1;
	DirectX::XMFLOAT4 Col;
	DirectX::XMFLOAT2 Texture0;
	DirectX::XMFLOAT3 Normal;
};

class Shape
{
private:

public:

	POS_COL_TEX_NORM_VERTEX vertices[];


};

