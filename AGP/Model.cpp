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
	if (m_ModelTexture) m_ModelTexture->Release();
	if (m_ModelSampler) m_ModelSampler->Release();

}

//
// need to change thi to use the vs_4_0 instead of the version 5 one 
HRESULT Model::LoadObjModel(char* filename)
{
	m_Object = new ObjFileModel(filename, m_D3DDevice, m_ImmediateContext);
	if (m_Object->filename == "FILE NOT LOADED") return S_FALSE;

	//created for handling the model vertex shader, pixel shader and any model errors
	HRESULT HR = S_OK;
	ID3DBlob* MVS, * MPS, * MError;

	D3DX11CreateShaderResourceViewFromFile(m_D3DDevice, "assets/texture.png", NULL, NULL, &m_ModelTexture, NULL);


	//creating model vertex shader
	HR = D3DX11CompileFromFile("ModelShader.hlsl", 0, 0, "ModelVS", "vs_5_0", 0, 0, 0, &MVS, &MError, 0);
	//checks to see if there was an error when creating
	if (MError != 0) // check the shader if this happens
	{
		OutputDebugStringA((char*)MError->GetBufferPointer());
		MError->Release();
		if (FAILED(HR))
		{
			return HR;
		}
	}

	//creating model pixel shader
	HR = D3DX11CompileFromFile("ModelShader.hlsl", 0, 0, "ModelPS", "ps_5_0", 0, 0, 0, &MPS, &MError, 0);
	//checks to see if there was an error when creating
	if (MError != 0) // check the shader if this happens
	{
		OutputDebugStringA((char*)MError->GetBufferPointer());
		MError->Release();
		if (FAILED(HR))
		{
			return HR;
		}
	}

	//create the shader object (Vertex)
	HR = m_D3DDevice->CreateVertexShader(MVS->GetBufferPointer(), MVS->GetBufferSize(), NULL, &m_VShader);
	//checks to see if failed
	if (FAILED(HR))
	{
		return HR;
	}

	//create the shder object (Pixel)
	HR = m_D3DDevice->CreatePixelShader(MPS->GetBufferPointer(), MPS->GetBufferSize(), NULL, &m_PShader);
	//checks to see if failed
	if (FAILED(HR))
	{
		return HR;
	}

	m_ImmediateContext->VSSetShader(m_VShader, 0, 0);
	m_ImmediateContext->PSSetShader(m_PShader, 0, 0);

	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		// make sure the corect dxgi and d3d version are set 
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HR = m_D3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), MVS->GetBufferPointer(), MVS->GetBufferSize(), &m_InputLayout);
	if (FAILED(HR))
	{
		return HR;
	}





	m_ImmediateContext->IASetInputLayout(m_InputLayout);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_D3DDevice->CreateSamplerState(&sampler_desc, &m_ModelSampler);


	D3D11_BUFFER_DESC CBuffer;
	ZeroMemory(&CBuffer, sizeof(CBuffer));
	CBuffer.Usage = D3D11_USAGE_DEFAULT;
	CBuffer.ByteWidth = 64;
	CBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HR = m_D3DDevice->CreateBuffer(&CBuffer, NULL, &m_constantBuffer);
	if (FAILED(HR)) return HR;


	//collision stuff
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();
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

void Model::Draw(DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection)
{
	DirectX::XMMATRIX world;

	D3DX11CreateShaderResourceViewFromFile(m_D3DDevice, "assets/texture.png", NULL, NULL, &m_ModelTexture, NULL);


	m_ImmediateContext->PSSetSamplers(0, 1, &m_ModelSampler);
	m_ImmediateContext->PSGetShaderResources(0, 1, &m_ModelTexture);

	world = DirectX::XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	world *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_xAngle));
	world *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_yAngle));
	world *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_zAngle));

	world *= DirectX::XMMatrixTranslation(m_x, m_y, m_z);

	m_CB1Values.WorldViewProjection = world * (*view) * (*projection);

	m_ImmediateContext->UpdateSubresource(m_constantBuffer, 0, 0, &m_CB1Values, 0, 0);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

	m_ImmediateContext->VSSetShader(m_VShader, 0, 0);
	m_ImmediateContext->PSSetShader(m_PShader, 0, 0);
	m_ImmediateContext->IASetInputLayout(m_InputLayout);

	m_Object->Draw();

}

void Model::SetXPos(float x)
{
	m_x = x;
}

void Model::SetYPos(float y)
{
	m_y = y;
}

void Model::SetZPos(float z)
{
	m_z = z;
}

float Model::GetXPos()
{
	return m_x;
}

float Model::GetYPos()
{
	return m_y;
}

float Model::GetZPos()
{
	return m_z;
}

void Model::SetXAngle(float x)
{
	m_xAngle = x;
}

void Model::SetYAngle(float y)
{
	m_yAngle = y;
}

void Model::SetZAngle(float z)
{
	m_zAngle = z;
}

void Model::SetScale(float scale)
{
	m_Scale = scale;
}

float Model::GetScale()
{
	return m_Scale;
}

void Model::LookAt_XZ(float x, float z)
{
	float dx, dz;

	dx = m_x - x;
	dz = m_z - z;

	m_yAngle = atan2(dx, dz) * (180 / DirectX::XM_PI);
}

void Model::MoveFoward(float distance)
{
	m_x += (m_yAngle * (DirectX::XM_PI / 180)) * distance;
	m_z += (m_yAngle * (DirectX::XM_PI / 180)) * distance;
}

float Model::GetBoundingSphereRadius()
{
	return BoundingSphereRadius * m_Scale;
}

DirectX::XMVECTOR Model::GetBoudningSphereWorldSpacePosition()
{
	DirectX::XMMATRIX world;

	world = DirectX::XMMatrixScaling(m_Scale, m_Scale, m_Scale);
	world *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_xAngle));
	world *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_yAngle));
	world *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_zAngle));

	world *= DirectX::XMMatrixTranslation(m_x, m_y, m_z);

	DirectX::XMVECTOR offset;

	offset = DirectX::XMVectorSet(BoundingSphereCentreX, BoundingSphereCentreY, BoundingSphereCentreZ, 0.0f);
	offset = DirectX::XMVector3Transform(offset, world);

	return offset;
}

bool Model::Checkcollision(Model* model)
{
	//if this is the same model then break out
	if (this == model)
	{
		return false;
	}

	DirectX::XMVECTOR CurrentModelPosition = this->GetBoudningSphereWorldSpacePosition();
	DirectX::XMVECTOR OtherModelPosition = model->GetBoudningSphereWorldSpacePosition();

	float VectorX1 = DirectX::XMVectorGetX(CurrentModelPosition);
	float VectorX2 = DirectX::XMVectorGetX(OtherModelPosition);
	float VectorY1 = DirectX::XMVectorGetY(CurrentModelPosition);
	float VectorY2 = DirectX::XMVectorGetY(OtherModelPosition);
	float VectorZ1 = DirectX::XMVectorGetZ(CurrentModelPosition);
	float VectorZ2 = DirectX::XMVectorGetZ(OtherModelPosition);

	float Distance_Between = pow(VectorX1 - VectorX2, 2) + pow(VectorY1 - VectorY2, 2) + pow(VectorZ1 - VectorZ2, 2);

	float RadiusPositions = this->GetBoundingSphereRadius() + model->GetBoundingSphereRadius();

	if (RadiusPositions > Distance_Between)
	{
		return true;
	}
	else
		return false;

}

