#include "Model.h"

Model::Model()
{
}

Model::Model(ID3D11Device* device, ID3D11DeviceContext* context)
	:m_D3DDevice{ device }, m_ImmediateContext{context}
{
	m_x, m_y, m_z = 0.0f;
	m_xAngle, m_yAngle, m_zAngle = 0;
	m_Scale = 1.0f;

}

Model::~Model()
{
	delete m_Object;

	if (m_InputLayout) m_InputLayout->Release();
	if (m_VShader) m_VShader->Release();
	if (m_PShader) m_PShader->Release();
	if (m_ImmediateContext) m_ImmediateContext->Release();
	if (m_D3DDevice) m_D3DDevice->Release();
	if (m_constantBuffer) m_constantBuffer->Release();

}

void Model::CalculateModelCentrePoint()
{
	float MinX{ 0 };
	float MaxX{ 0 };
	float MinY{ 0 };
	float MaxY{ 0 };
	float MinZ{ 0 };
	float MaxZ{ 0 };

	for (int i = 0; i < m_Object->numverts; ++i)
	{
		if (m_Object->vertices[i].Pos.x > MaxX)
		{
			MaxX = m_Object->vertices[i].Pos.x;
		}
		//test for maximum x
		else if (m_Object->vertices[i].Pos.x < MinX)
		{
			MinX = m_Object->vertices[i].Pos.x;
		}
		//test for minimum y
		else if (m_Object->vertices[i].Pos.y > MaxY)
		{
			MaxY = m_Object->vertices[i].Pos.y;
		}
		// test for maximum y
		else if (m_Object->vertices[i].Pos.y < MinY)
		{
			MinY = m_Object->vertices[i].Pos.y;
		}
		// test for minimum z
		else if (m_Object->vertices[i].Pos.z > MaxZ)
		{
			MaxZ = m_Object->vertices[i].Pos.z;
		}
		//test for maximum z
		else if (m_Object->vertices[i].Pos.z < MinZ)
		{
			MinZ = m_Object->vertices[i].Pos.z;
		}
	}

	BoundingSphereCentreX = MaxX - MinX;
	BoundingSphereCentreY = MaxY - MinY;
	BoundingSphereCentreZ = MaxZ - MinZ;

}

void Model::CalculateBoundingSphereRadius()
{
	float HighestDistanceFromCentre = 0;

	float SquaredCureentDistanceFromCentre;

	for (int i = 0; i < m_Object->numverts; i++)
	{
		SquaredCureentDistanceFromCentre = pow(BoundingSphereCentreX, 2) + pow(m_Object->vertices[i].Pos.x, 2);

		if (SquaredCureentDistanceFromCentre > HighestDistanceFromCentre)
		{
			HighestDistanceFromCentre = SquaredCureentDistanceFromCentre;
		}
	}

	BoundingSphereRadius = HighestDistanceFromCentre;
}