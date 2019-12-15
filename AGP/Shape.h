#pragma once
#include <DirectXMath.h>

// the structure the arrway will take when it comes to shaders
struct POS_COL_TEX_NORM_VERTEX
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

	// the constructor
	Shape();
	
	//the Destructor
	~Shape();

	// the verticies for making a 3d cube
	POS_COL_TEX_NORM_VERTEX vertices[];

	//returns the size the structure declared above
	int SizeOfPOSStruct();

};

