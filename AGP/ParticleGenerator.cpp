#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
	ParticleDevice = device;
	ParticleImmediateContext = immediatecontext;
	ParticleX = 0;
	ParticleY = 0;
	ParticleZ = 0;
	ParticleAngleX = 0;
	ParticleAngleY = 0;
	ParticleAngleZ = 0;
	ParticleScale = 1.0f;


	for (int i = 0; i < 100; i++)
	{
		Particle* NewParticle = new Particle();
		
		NewParticle->colour = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		NewParticle->gravity = 1;
		NewParticle->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		NewParticle->velocity = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		NewParticle->age = 30;

		Free.push_back(NewParticle);
	}

}

ParticleGenerator::~ParticleGenerator()
{
	if (ParticleDevice)				ParticleDevice->Release();
	if (ParticleImmediateContext)	ParticleImmediateContext->Release();
	if (ParticleVShader)			ParticleVShader->Release();
	if (ParticlePShader)			ParticlePShader->Release();
	if (ParticleInputLayout)		ParticleInputLayout->Release();
	if (ParticleConstantBuffer)		ParticleConstantBuffer->Release();

	Free.clear();
	active.clear();

}

float ParticleGenerator::RandomZeroToOne()
{
	//return  srand(float(timeGetTime()) / 1000.0f) % 1;
	return rand() % 2;
}

float ParticleGenerator::RandomNegOneToPosOne()
{
	return rand() % 1 + -1;
}

HRESULT ParticleGenerator::CreateParticle()
{
	HRESULT hr = S_OK;
	DirectX::XMFLOAT3 vertices[6] =
	{
		DirectX::XMFLOAT3(-1.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3( 1.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3( 1.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3( 1.0f, 1.0f, 0.0f),
	};

	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;//sets no culling
	hr = ParticleDevice->CreateRasterizerState(&rasterizer_desc, &RasterSolid);
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;//sets backface culling 
	hr = ParticleDevice->CreateRasterizerState(&rasterizer_desc, &RasterParticle);

	//create the vert buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//both cpu and gpu
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * 6/*VERTCOUNT*/;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//edit on the cpu

	hr = ParticleDevice->CreateBuffer(&bufferDesc, NULL, &ParticleVertexBuffer);

	if (FAILED(hr)) return 0;

	//copy verts to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//mapping = locking the buffer which allows writing
	ParticleImmediateContext->Map(ParticleVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));//copy data
	//unlock the buffer
	ParticleImmediateContext->Unmap(ParticleVertexBuffer, NULL);

	//creating and setting up model constant buffer
	D3DX11CreateShaderResourceViewFromFile(ParticleDevice, "assets/robotexture.bmp", NULL, NULL, &ParticleTexture, NULL);

	//loading in the vertex and pixel shaders of the model
	ID3DBlob* VS1, * PS1, * Error1;

	//finding the vertex shader
	hr = D3DX11CompileFromFile("particlegenerator.hlsl", 0, 0, "ParticleVS", "vs_4_0", 0, 0, 0, &VS1, &Error1, 0);

	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}

	//finding the pixel shader
	hr = D3DX11CompileFromFile("particlegenerator.hlsl", 0, 0, "ParticlePS", "ps_4_0", 0, 0, 0, &PS1, &Error1, 0);

	if (Error1 != 0)
	{
		OutputDebugStringA((char*)Error1->GetBufferPointer());
		Error1->Release();
		if (FAILED(hr))
			return hr;
	}

	//create the shader objects
	hr = ParticleDevice->CreateVertexShader(VS1->GetBufferPointer(), VS1->GetBufferSize(), NULL, &ParticleVShader);

	if (FAILED(hr))
		return hr;

	hr = ParticleDevice->CreatePixelShader(PS1->GetBufferPointer(), PS1->GetBufferSize(), NULL, &ParticlePShader);

	if (FAILED(hr))
		return hr;

	//set up the shader objects as active
	ParticleImmediateContext->VSSetShader(ParticleVShader, 0, 0);
	ParticleImmediateContext->PSSetShader(ParticlePShader, 0, 0);

	//create and set the input layout
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = ParticleDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS1->GetBufferPointer(), VS1->GetBufferSize(), &ParticleInputLayout);

	if (FAILED(hr))
		return hr;

	ParticleImmediateContext->IASetInputLayout(ParticleInputLayout);

	//constant buffer for model
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = 80;
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = ParticleDevice->CreateBuffer(&constant_buffer_desc, NULL, &ParticleConstantBuffer);

	if (FAILED(hr))
	{
		return hr;
	}



	return hr;


}


void ParticleGenerator::SetEngine(bool OnOff)
{
	Active = OnOff;
}

void ParticleGenerator::DrawParticle(DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection, DirectX::XMFLOAT3* cameraposition)
{
	float TimeNow = float(timeGetTime()) / 1000.0f;
	float deltatime = TimeNow - TimePrevious;
	TimePrevious = TimeNow;
	UntilParticle -= deltatime;
	
	UINT stride = sizeof(DirectX::XMFLOAT3);
	PARTICLE_CONSTANT_BUFFER PCB_Values;
	
	DirectX::XMMATRIX world;
	UINT offset = 0;
	
	float m_age = 0;
	float m_untilParticle = 0;
	
	if (UntilParticle <= 0.0f)
	{
		if (Active)//a bool to check if the particle engine is on or off. Make a getter/setter and use it in main
		{
			it = Free.begin();//point to the beggining of the free list
	
			//add a new particle to the back of m_active from the front of m_free
			if (Free.size() != NULL)
			{
				switch (TYPE)//the name of my enum
				{
				case RAINBOW_FOUNTAIN:
				{
					m_age = 4.0f;
					m_untilParticle = 0.008f;
					////////////////////////initialise the particle NOTE: all of this is adjustable for different effects////////////////////////
					(*it)->colour = DirectX::XMFLOAT4(RandomZeroToOne(), RandomZeroToOne(), RandomZeroToOne(), 1.0f);
					(*it)->gravity = 4.5f;
					(*it)->position = DirectX::XMFLOAT3(-10.0f, 1.0f, 3.0f);
					(*it)->velocity = DirectX::XMFLOAT3(10.0f, 5.0f, 10.0f);
						////////////////////////////////////////////////////////////////////////////////////////////////
						break;
				}
				default:
				{
					break;
				}
				}
				(*it)->age -= 1.0f;//set age to 0. this is used for knowing when to delete the particle
	
			//////add the particle from the front of the available list to the back of the active list and remove it
				active.push_back(*it);
				Free.pop_front();
			}
		}
		else UntilParticle = 0.001f;
	}
	
	
	
	if (active.size() != NULL)//safety check
	{
		it = active.begin();//point the iterator to the front of the active list ready for processing
		while (it != active.end())//move all of the particles
		{
	
			switch (TYPE)
			{
				case RAINBOW_FOUNTAIN:
				{
					
					(*it)->age += deltatime;
					(*it)->velocity.y -= (*it)->gravity * (deltatime);
					(*it)->position.x += (*it)->velocity.x * (deltatime);
					(*it)->position.y += (*it)->velocity.y * (deltatime);
					(*it)->position.z += (*it)->velocity.z * (deltatime);
					
					break;
				}
				default:
				{
					break;
				}
			}
	
			world = DirectX::XMMatrixIdentity();
			
			switch (TYPE)
			{
				case RAINBOW_FOUNTAIN:
				{
					/*set scale and world transforms here*/
					world *= DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f);
					world *= DirectX::XMMatrixRotationY(180 * (DirectX::XM_PI / 180));
					world *= DirectX::XMMatrixTranslation((*it)->position.x, (*it)->position.y, (*it)->position.z);
					break;
				}
				default:
				{
					break;
				}
			}
			
	
			//constant buffer stuff for shader
			PCB_Values.WorldViewProjection = (world) * (*view) * (*projection);
			PCB_Values.Color = (*it)->colour;
			//PCB_Values.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

			ParticleImmediateContext->VSSetConstantBuffers(0, 1, &ParticleConstantBuffer);
			ParticleImmediateContext->UpdateSubresource(ParticleConstantBuffer, 0, 0, &PCB_Values, 0, 0);
	
			//set the shader objects as active
			ParticleImmediateContext->VSSetShader(ParticleVShader, 0, 0);
			ParticleImmediateContext->PSSetShader(ParticlePShader, 0, 0);
			ParticleImmediateContext->IASetInputLayout(ParticleInputLayout);
			
			ParticleImmediateContext->IASetVertexBuffers(0, 1, &ParticleVertexBuffer, &stride, &offset);
			
			ParticleImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			ParticleImmediateContext->PSSetSamplers(0, 1, &ParticleSampler);
			ParticleImmediateContext->PSSetShaderResources(0, 1, &ParticleTexture);
			
			ParticleImmediateContext->RSSetState(RasterSolid);//set backface culling to on
			ParticleImmediateContext->Draw(6, 0);//draw the particle
			ParticleImmediateContext->RSSetState(RasterSolid);//set backface culling to off

			if ((*it)->age >= m_age)//check the age of the current particle
			{
				it++;
				active.front()->age = m_age;
				active.front()->position = { (RandomNegOneToPosOne() + active.front()->position.x * 10) * (RandomZeroToOne() * 10), active.front()->position.y + 5.0f, cameraposition->z + 7.0f };
				active.front()->velocity = { 0.0f, 4.50f, RandomNegOneToPosOne() };
				Free.push_back(active.front());//move the (now previously) current active particle to the back of the pool			
				active.pop_front();//remove the particle			
			}
			else it++;
		}//end of while
	}//end of if(m_active.size()!=NULL

	

}

void ParticleGenerator::DrawOne(Particle* one, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, DirectX::XMFLOAT3 cameraposition)
{
	UINT stride = sizeof(DirectX::XMFLOAT3);
	PARTICLE_CONSTANT_BUFFER PCB_Values;

	DirectX::XMMATRIX world;

	world *= DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f);
	world *= DirectX::XMMatrixRotationY(DirectX::XM_PI / 180);
	world *= DirectX::XMMatrixTranslation(one->position.x, one->position.y, one->position.z);

	PCB_Values.WorldViewProjection = world * (view) * (projection);
	PCB_Values.Color = one->colour;

	ParticleImmediateContext->VSSetConstantBuffers(0, 1, &ParticleConstantBuffer);
	ParticleImmediateContext->PSSetConstantBuffers(0, 1, &ParticleConstantBuffer);

	ParticleImmediateContext->UpdateSubresource(ParticleConstantBuffer, 0, 0, &ConstantBufferValues, 0, 0);

	ParticleImmediateContext->RSSetState(RasterParticle);
	ParticleImmediateContext->OMSetDepthStencilState(DepthWriteParticle, 0);

	ParticleImmediateContext->PSSetSamplers(0, 1, &ParticleSampler);
	ParticleImmediateContext->PSSetShaderResources(0, 1, &ParticleTexture);


	ParticleImmediateContext->Draw(6, 0);
	ParticleImmediateContext->RSSetState(RasterSolid);
	ParticleImmediateContext->OMSetDepthStencilState(DepthWriteSolid, 0);
}


void ParticleGenerator::SetX(float X)
{
	ParticleX = X;
}

void ParticleGenerator::SetY(float Y)
{
	ParticleY = Y;
}

void ParticleGenerator::SetZ(float Z)
{
	ParticleZ = Z;
}

void ParticleGenerator::SetXAngle(float x)
{
	ParticleAngleX = x;
}


void ParticleGenerator::SetYAngle(float y)
{
	ParticleAngleY = y;
}

void ParticleGenerator::SetZAngle(float z)
{
	ParticleAngleZ = z;
}

void ParticleGenerator::SetScale(float scale)
{
	ParticleScale = scale;
}

float ParticleGenerator::ReturnXPos()
{
	return ParticleX;
}

float ParticleGenerator::ReturnYPos()
{
	return ParticleY;
}

float ParticleGenerator::ReturnZPos()
{
	return ParticleZ;
}

float ParticleGenerator::ReturnXAngle()
{
	return ParticleAngleX;
}

float ParticleGenerator::ReturnYAngle()
{
	return ParticleAngleY;
}

float ParticleGenerator::ReturnZAngle()
{
	return ParticleAngleZ;
}

float ParticleGenerator::ReturnScale()
{
	return ParticleScale;
}

void ParticleGenerator::ChangeXPos(float ChangeAmount)
{
	ParticleX += ChangeAmount;
}

void ParticleGenerator::ChangeYPos(float ChangeAmount)
{
	ParticleY += ChangeAmount;
}

void ParticleGenerator::ChangeZPos(float ChangeAmount)
{
	ParticleZ += ChangeAmount;
}

void ParticleGenerator::ChangeXAngle(float ChangeAmount)
{
	ParticleAngleX += ChangeAmount;
}

void ParticleGenerator::ChangeYAngle(float ChangeAmount)
{
	ParticleAngleY += ChangeAmount;
}

void ParticleGenerator::ChangeZAngle(float ChangeAmount)
{
	ParticleAngleZ += ChangeAmount;
}

void ParticleGenerator::ChangeScale(float ChangeAmount)
{
	ParticleScale += ChangeAmount;
}

void ParticleGenerator::LookAt_XZ(float WorldSpaceX, float WorldSpaceZ)
{
	float DX, DZ;
	DX = ParticleX - WorldSpaceX;
	DZ = ParticleZ - WorldSpaceZ;

	ParticleAngleY = atan2(DX, DZ) * (180 / DirectX::XM_PI);

}

void ParticleGenerator::MoveFoward(float distance)
{
	ParticleX += sin(ParticleAngleY * (DirectX::XM_PI / 180)) * distance;
	ParticleZ += cos(ParticleAngleY * (DirectX::XM_PI / 180)) * distance;

}

void ParticleGenerator::MoveSideWays(float distance)
{
	ParticleX += distance;
}

