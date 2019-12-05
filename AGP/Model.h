#pragma once
#include "objfilemodel.h"
#include <DirectXMath.h>



struct CONSTANT_BUFFER1
{
	DirectX::XMMATRIX WorldViewProjection;
};

struct POS_COL_TEX_NORM_VERTEX //this willbe added to and renamed in future tutorials
{
	DirectX::XMFLOAT3 vertex1;
	DirectX::XMFLOAT4 Col;
	DirectX::XMFLOAT2 Texture0;
	DirectX::XMFLOAT3 Normal;
};


class Model
{
private:
	//the constant buffer from the Model shader
	CONSTANT_BUFFER1 m_CB1Values;

	//directX stuff
	ID3D11Device* m_D3DDevice;
	ID3D11DeviceContext* m_ImmediateContext;

	// the object file loader
	ObjFileModel* m_Object;

	//shaders
	ID3D11VertexShader* m_VShader;
	ID3D11PixelShader* m_PShader;

	ID3D11InputLayout* m_InputLayout;
	ID3D11Buffer* m_constantBuffer;

	//this may not work as im trying something differnt to the tutroial(this normall was made within render?)
	ID3D11ShaderResourceView* m_ModelTexture;
	ID3D11SamplerState* m_ModelSampler;

	float m_x, m_y, m_z;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_Scale;

	float BoundingSphereCentreX;
	float BoundingSphereCentreY;
	float BoundingSphereCentreZ;
	float BoundingSphereRadius;

	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();

public:

	Model();
	Model(ID3D11Device* device, ID3D11DeviceContext* context);
	~Model();

	HRESULT LoadObjModel(char* filename);

	void Draw(DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection);

	void SetXPos(float X);
	void SetYPos(float X);
	void SetZPos(float X);

	void SetXAngle(float x);
	void SetYAngle(float y);
	void SetZAngle(float z);

	void SetScale(float scale);

	float GetXPos();
	float GetYPos();
	float GetZPos();

	float GetScale();

	void LookAt_XZ(float x, float z);
	void MoveFoward(float distance);

	float GetBoundingSphereRadius();
	DirectX::XMVECTOR GetBoudningSphereWorldSpacePosition();

	bool Checkcollision(Model* model);


	POS_COL_TEX_NORM_VERTEX vertices[];

};

