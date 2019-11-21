#pragma once
#include "objfilemodel.h"
#include <DirectXMath.h>



struct CONSTANT_BUFFER1
{
	DirectX::XMMATRIX WorldViewProjection;
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

	void Draw(DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection, ID3D11SamplerState* sampler, ID3D11ShaderResourceView* texture);

	void SetXpos(float X);
	void SetYpos(float X);
	void Setzpos(float X);

	void SetXAngle(float x);
	void SetYAngle(float y);
	void SetZAngle(float z);

	void SetScale(float scale);

	float GetXpos();
	float GetYpos();
	float GetZpos();

	float GetScale();

	void LookAt_XZ(float x, float z);
	void MoveFoward(float distance);

	float GetBoundingSphereRadius();
	DirectX::XMVECTOR GetBoudningSphereWorldSpacePosition();

	bool Checkcollision(Model* model);

};

