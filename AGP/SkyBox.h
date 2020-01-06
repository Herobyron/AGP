#include "Model.h"
#include <directxmath.h>

//the constant buffer struct for the sky boc
struct SkyBox_CONSTANT_BUFFER
{
	DirectX::XMMATRIX WorldViewProjection;	// 64 bytes
};


struct SKYVout
{
	DirectX::XMFLOAT3 Pos; //12
	DirectX::XMFLOAT4 Col; //16
	DirectX::XMFLOAT2 Texture; //8
	DirectX::XMFLOAT3 Normal; // 12
};


class SkyBox
{
private:

	//refernce to the device and immediate context created in setup.
	ID3D11Device* SkyBoxDevice;
	ID3D11DeviceContext* SkyBoxImmediateContext;

	//constant buffer struct
	SkyBox_CONSTANT_BUFFER ConstantBufferValues;

	//creates a refernce of ObjFileModel
	Model* SkyBoxObject;

	// the skybox vertex shader
	ID3D11VertexShader* SkyBoxVShader;

	// the skybox pixel shader
	ID3D11PixelShader* SkyBoxPShader;

	//the skybox input layout
	ID3D11InputLayout* SkyBoxInputLayout;

	// the skybox constant buffer
	ID3D11Buffer* SkyBoxConstantBuffer;

	// the skybox x, y, z position
	float SkyBoxX, SkyBoxY, SkyBoxZ;

	// the skybox angles on the x, y and z axis
	float SkyBoxAngleX, SkyBoxAngleY, SkyBoxAngleZ;

	// the skybox scale
	float SkyBoxScale;

	//member variables needed for sky box
	ID3D11RasterizerState* RasterSolid = 0;
	ID3D11RasterizerState* RasterSkyBox = 0;
	ID3D11DepthStencilState* DepthWriteSolid = 0;
	ID3D11DepthStencilState* DepthWriteSkyBox = 0;

	//skybox sampler and shader
	ID3D11SamplerState* SkyBoxSampler;
	ID3D11ShaderResourceView* SkyBoxTexture;
	ID3D11Buffer* SkyBoxVertexBuffer;

public:

	//model constructor
	// the constructor takes two parametes. a refernce to the device
	// and a refernce to the immediate context which were created in setup
	SkyBox(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

	//model destructor
	~SkyBox();

	//a function that loads the object into the game
	HRESULT SkyBoxInitialisation();

	// a function to actually draw the model to the screen
	void Draw(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	//functions to set all of the positions of the model
	void SetBoxX(float X);
	void SetBoxY(float Y);
	void SetBoxZ(float Z);

	//functions to set all of the rotations of the model
	void SetBoxXAngle(float X);
	void SetBoxYAngle(float Y);
	void SetBoxZAngle(float Z);

	// a function to set the scale of the object
	void SetBoxScale(float scale);

	//functions to get all of the positions of the model
	float ReturnXPos();
	float ReturnYPos();
	float ReturnZPos();

	//functions of get all of the rotations of the model
	float ReturnXAngle();
	float ReturnYAngle();
	float ReturnZAngle();

	// a function to get the scale of the model
	float ReturnScale();



};

