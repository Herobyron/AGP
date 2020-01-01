#include "Model.h"

Model::Model(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
	ModelDevice = device;
	ModelImmediateContext = immediatecontext;
	ModelX = 0;
	ModelY = 0;
	ModelZ = 0;
	ModelAngleX = 0;
	ModelAngleY = 0;
	ModelAngleZ = 0;
	ModelScale = 1.0f;
}

Model::~Model()
{
	if (ModelDevice) ModelDevice->Release();
	if (ModelImmediateContext) ModelImmediateContext->Release();
	if (ModelObject) delete ModelObject;
	if (ModelVShader) ModelVShader->Release();
	if (ModelPShader) ModelPShader->Release();
	if (ModelInputLayout) ModelInputLayout->Release();
	if (ModelConstantBuffer) ModelConstantBuffer->Release();

}

HRESULT Model::LoadObjModel(char* FileName)
{
	HRESULT hr = S_OK;

	ModelObject = new ObjFileModel(FileName, ModelDevice, ModelImmediateContext);
	if (ModelObject->filename == "FILE NOT LOADED")
	{
		return S_FALSE;
	}

	//creating and setting up model constant buffer


	//loading in the vertex and pixel shaders of the model
	ID3DBlob* VS1, * PS1, * Error1;

	//finding the vertex shader
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS1, &Error1, 0);

	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}

	//finding the pixel shader
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS1, &Error1, 0);

	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}

	//create the shader objects
	hr = ModelDevice->CreateVertexShader(VS1->GetBufferPointer(), VS1->GetBufferSize(), NULL, &ModelVShader);

	if (FAILED(hr))
		return hr;

	hr = ModelDevice->CreatePixelShader(PS1->GetBufferPointer(), PS1->GetBufferSize(), NULL, &ModelPShader);

	if (FAILED(hr))
		return hr;

	//set up the shader objects as active
	ModelImmediateContext->VSSetShader(ModelVShader, 0, 0);
	ModelImmediateContext->PSSetShader(ModelPShader, 0, 0);

	//create and set the input layout
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = ModelDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS1->GetBufferPointer(), VS1->GetBufferSize(), &ModelInputLayout);

	if (FAILED(hr))
		return hr;

	ModelImmediateContext->IASetInputLayout(ModelInputLayout);

	//constant buffer for model
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = 64;
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = ModelDevice->CreateBuffer(&constant_buffer_desc, NULL, &ModelConstantBuffer);

	if (FAILED(hr))
	{
		return hr;
	}


	// calculating the model centre points and radius
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

	return hr;

}

void Model::Draw(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	MODEL_CONSTANT_BUFFER MCB_Values;
	DirectX::XMMATRIX world, scale, rotate, translate;

	//when calcualte remember is scale x rotate x translate 
	//(rotation may be off as ive done something new. if dosent work do each rotation manually)
	scale = DirectX::XMMatrixScaling(ModelScale, ModelScale, ModelScale);
	rotate = DirectX::XMMatrixRotationRollPitchYaw(ModelAngleX, ModelAngleY, ModelAngleZ);
	translate = DirectX::XMMatrixTranslation(ModelX, ModelY, ModelZ);

	world = scale * rotate * translate;

	MCB_Values.WorldViewProjection = world * (view) * (projection);

	//set the constant buffer to be active
	ModelImmediateContext->VSSetConstantBuffers(0, 1, &ModelConstantBuffer);

	//update the constant buffer resource
	ModelImmediateContext->UpdateSubresource(ModelConstantBuffer, 0, 0, &MCB_Values, 0, 0);


	//set the shader objects as active
	ModelImmediateContext->VSSetShader(ModelVShader, 0, 0);
	ModelImmediateContext->PSSetShader(ModelPShader, 0, 0);
	ModelImmediateContext->IASetInputLayout(ModelInputLayout);

	ModelObject->Draw();

}

void Model::SetX(float X)
{
	ModelX = X;
}

void Model::SetY(float Y)
{
	ModelY = Y;
}

void Model::SetZ(float Z)
{
	ModelZ = Z;
}

void Model::SetXAngle(float x)
{
	ModelAngleX = x;
}


void Model::SetYAngle(float y)
{
	ModelAngleY = y;
}

void Model::SetZAngle(float z)
{
	ModelAngleZ = z;
}

void Model::SetScale(float scale)
{
	ModelScale = scale;
}

float Model::ReturnXPos()
{
	return ModelX;
}

float Model::ReturnYPos()
{
	return ModelY;
}

float Model::ReturnZPos()
{
	return ModelZ;
}

float Model::ReturnXAngle()
{
	return ModelAngleX;
}

float Model::ReturnYAngle()
{
	return ModelAngleY;
}

float Model::ReturnZAngle()
{
	return ModelAngleZ;
}

float Model::ReturnScale()
{
	return ModelScale;
}

void Model::ChangeXPos(float ChangeAmount)
{
	ModelX += ChangeAmount;
}

void Model::ChangeYPos(float ChangeAmount)
{
	ModelY += ChangeAmount;
}

void Model::ChangeZPos(float ChangeAmount)
{
	ModelZ += ChangeAmount;
}

void Model::ChangeXAngle(float ChangeAmount)
{
	ModelAngleX += ChangeAmount;
}

void Model::ChangeYAngle(float ChangeAmount)
{
	ModelAngleY += ChangeAmount;
}

void Model::ChangeZAngle(float ChangeAmount)
{
	ModelAngleZ += ChangeAmount;
}

void Model::ChangeScale(float ChangeAmount)
{
	ModelScale += ChangeAmount;
}

void Model::LookAt_XZ(float WorldSpaceX, float WorldSpaceZ)
{
	float DX, DZ;
	DX = ModelX - WorldSpaceX;
	DZ = ModelZ - WorldSpaceZ;

	ModelAngleY = atan2(DX, DZ) * (180 / DirectX::XM_PI);

}

void Model::MoveFoward(float distance)
{
	ModelX += sin(ModelAngleY * (DirectX::XM_PI / 180)) * distance;
	ModelZ += cos(ModelAngleY * (DirectX::XM_PI / 180)) * distance;

}

void Model::MoveSideWays(float distance)
{
	ModelX +=  distance;
}

void Model::CalculateModelCentrePoint()
{
	float MinX = {0};
	float MaxX = {0};
	float MinY = {0};
	float MaxY = {0};
	float MinZ = {0};
	float MaxZ = {0};


	for (int i = 0; i < ModelObject->numverts; i++)
	{
		// test for min and max X
		if (ModelObject->vertices[i].Pos.x >= MaxX)
		{
			MaxX = ModelObject->vertices[i].Pos.x;
			MaxX *= ModelScale;
		}

		if (ModelObject->vertices[i].Pos.x <= MinX)
		{
			MinX = ModelObject->vertices[i].Pos.x;
			MinX *= ModelScale;
		}

		//test for the min and max of the y position
		if (ModelObject->vertices[i].Pos.y >= MaxY)
		{
			MaxY = ModelObject->vertices[i].Pos.y;
			MaxY *= ModelScale;
		}

		if (ModelObject->vertices[i].Pos.y <= MinY)
		{
			MinY = ModelObject->vertices[i].Pos.y;
			MinY *= ModelScale;
		}

		//tests for the min and max of the z position
		if (ModelObject->vertices[i].Pos.z >= MaxZ)
		{
			MaxZ = ModelObject->vertices[i].Pos.z;
			MaxZ *= ModelScale;
		}

		if (ModelObject->vertices[i].Pos.z <= MinZ)
		{
			MinZ = ModelObject->vertices[i].Pos.z;
			MinZ *= ModelScale;
		}

		BoundingSphereCentreX = ((MaxX - MinX) / 2);
		BoundingSphereCentreY = ((MaxY - MinY) / 2);
		BoundingSphereCentreZ = ((MaxZ - MinZ) / 2);

		float ChangeDistX = (BoundingSphereCentreX * ModelScale);
		float ChangeDistY = (BoundingSphereCentreY * ModelScale);
		float ChangeDistZ = (BoundingSphereCentreZ * ModelScale);

		BoundingSphereCentreX -= ChangeDistX;
		BoundingSphereCentreY -= ChangeDistY;
		BoundingSphereCentreZ -= ChangeDistZ;

	}
}


void Model::CalculateBoundingSphereRadius()
{
	float HighestDistanceFromCentre = 0;
	float SquaredCurentdistanceFromCentreX;
	float SquaredCurrentDistanceFromCentreY;
	float SquredCurrentDistanceFromCentreZ;



	for (int i = 0; i < ModelObject->numverts; i++)
	{
		SquaredCurentdistanceFromCentreX =  pow(ModelObject->vertices[i].Pos.x, 2) + pow(BoundingSphereCentreX, 2);
		//new way for radius of sphere
		//SquaredCurentdistanceFromCentre = pow(ModelObject->vertices[i].Pos.x - BoundingSphereCentreX, 2) + pow(ModelObject->vertices[i].Pos.y - BoundingSphereCentreY, 2) + pow(ModelObject->vertices[i].Pos.z - BoundingSphereCentreZ, 2);
		
		// more new
		SquaredCurrentDistanceFromCentreY = pow(ModelObject->vertices[i].Pos.y, 2) + pow(BoundingSphereCentreY, 2);

		SquredCurrentDistanceFromCentreZ = pow(ModelObject->vertices[i].Pos.z, 2) + pow(BoundingSphereCentreZ, 2);

		if (SquaredCurentdistanceFromCentreX > HighestDistanceFromCentre)
		{
			HighestDistanceFromCentre = SquaredCurentdistanceFromCentreX;
		}
		
		if (SquaredCurrentDistanceFromCentreY > HighestDistanceFromCentre)
		{
			HighestDistanceFromCentre = SquaredCurrentDistanceFromCentreY;
		}

		if (SquredCurrentDistanceFromCentreZ > HighestDistanceFromCentre)
		{
			HighestDistanceFromCentre = SquredCurrentDistanceFromCentreZ;
		}

	}

	BoungingSphereRadius = HighestDistanceFromCentre ;
}

DirectX::XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	DirectX::XMMATRIX world, scale, rotate, translate;
	DirectX::XMVECTOR offset;

	scale = DirectX::XMMatrixScaling(ModelScale, ModelScale, ModelScale);
	rotate = DirectX::XMMatrixRotationRollPitchYaw(ModelAngleX, ModelAngleY, ModelAngleZ);
	translate = DirectX::XMMatrixTranslation(ModelX, ModelY, ModelZ);

	world = scale * rotate * translate;

	offset = DirectX::XMVectorSet(BoundingSphereCentreX, BoundingSphereCentreY, BoundingSphereCentreZ, 0.0);

	offset = DirectX::XMVector3Transform( offset, world);

	return offset;
}

float Model::GetBoundingSphereRadius()
{
	return BoungingSphereRadius * ModelScale;
}

bool Model::CheckCollision(Model* OtherModel)
{
	if (this == OtherModel)
	{
		return false;
	}

	DirectX::XMVECTOR CurrentModelPosition = this->GetBoundingSphereWorldSpacePosition();
	DirectX::XMVECTOR OtherModelPosition = OtherModel->GetBoundingSphereWorldSpacePosition();

	float ThisModelX = DirectX::XMVectorGetX(CurrentModelPosition);
	float ThisModelY = DirectX::XMVectorGetY(CurrentModelPosition);
	float ThisModelZ = DirectX::XMVectorGetZ(CurrentModelPosition);

	float OtherModelX = DirectX::XMVectorGetX(OtherModelPosition);
	float OtherModelY = DirectX::XMVectorGetY(OtherModelPosition);
	float OtherModelZ = DirectX::XMVectorGetZ(OtherModelPosition);

	float DistanceBetween = pow(ThisModelX - OtherModelX, 2) + pow(ThisModelY - OtherModelY, 2) + pow(ThisModelZ - OtherModelZ, 2);
	float SumOfRadius = this->GetBoundingSphereRadius() + OtherModel->GetBoundingSphereRadius();

	if ((DistanceBetween) < (SumOfRadius))
	{
		return true;
	}
	else
		return false;
}

bool Model::CheckCollisionAABB(Model* OtherModel)
{
	float ThisMinX = { 0 };
	float ThisMaxX = { 0 };
	float ThisMinY = { 0 };
	float ThisMaxY = { 0 };
	float ThisMinZ = { 0 };
	float ThisMaxZ = { 0 };

	float OtherMaxX = { 0 };
	float OtherMinX = { 0 };
	float OtherMaxY = { 0 };
	float OtherMinY = { 0 };
	float OtherMaxZ = { 0 };
	float OtherMinZ = { 0 };

	for (int i = 0; i < ModelObject->numverts; i++)
	{
		// test for min and max X
		if (ModelObject->vertices[i].Pos.x > ThisMaxX)
		{
			ThisMaxX = ModelObject->vertices[i].Pos.x;
		}
		else if (ModelObject->vertices[i].Pos.x < ThisMinX)
		{
			ThisMinX = ModelObject->vertices[i].Pos.x;
		}
		//test for the min and max of the y position
		if (ModelObject->vertices[i].Pos.y > ThisMaxY)
		{
			ThisMaxY = ModelObject->vertices[i].Pos.y;
		}
		else if (ModelObject->vertices[i].Pos.y < ThisMinY)
		{
			ThisMinY = ModelObject->vertices[i].Pos.y;
		}
		//tests for the min and max of the z position
		if (ModelObject->vertices[i].Pos.z > ThisMaxZ)
		{
			ThisMaxZ = ModelObject->vertices[i].Pos.z;
		}
		else if (ModelObject->vertices[i].Pos.z < ThisMinZ)
		{
			ThisMinZ = ModelObject->vertices[i].Pos.z;
		}
	}
		// for the other model
		for (int i = 0; i < OtherModel->ModelObject->numverts; i++)
		{
			// test for min and max X
			if (OtherModel->ModelObject->vertices[i].Pos.x > OtherMaxX)
			{
				OtherMaxX = OtherModel->ModelObject->vertices[i].Pos.x;
			}
			else if (OtherModel->ModelObject->vertices[i].Pos.x < OtherMinX)
			{
				OtherMinX = OtherModel->ModelObject->vertices[i].Pos.x;
			}
			//test for the min and max of the y position
			if (OtherModel->ModelObject->vertices[i].Pos.y > OtherMaxY)
			{
				OtherMaxY = OtherModel->ModelObject->vertices[i].Pos.y;
			}
			else if (OtherModel->ModelObject->vertices[i].Pos.y < OtherMinY)
			{
				OtherMinY = OtherModel->ModelObject->vertices[i].Pos.y;
			}
			//tests for the min and max of the z position
			if (OtherModel->ModelObject->vertices[i].Pos.z > OtherMaxZ)
			{
				OtherMaxZ = OtherModel->ModelObject->vertices[i].Pos.z;
			}
			else if (OtherModel->ModelObject->vertices[i].Pos.z < OtherMinZ)
			{
				OtherMinZ = OtherModel->ModelObject->vertices[i].Pos.z;
			}
		}


		// testing the actual collision
		if (ThisMaxX >= OtherMinX && ThisMinX <= OtherMaxX && ThisMinY <= OtherMaxY && ThisMaxY >= OtherMinY && ThisMinZ <= OtherMaxZ && ThisMaxZ >= OtherMinZ)
		{
			// they are colliding 
			return true;
		}

		return false;


	
}
