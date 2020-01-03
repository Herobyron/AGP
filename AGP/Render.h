#pragma once
#include "DirectSetUp.h"
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <directxmath.h>
// this will be moved to main and i will change initialise graphics to take a camera pointer so it can be changed position later on
#include "Camera.h" 
#include "Model.h"
#include "SkyBox.h"
#include "World.h"
#include "NPC.h"
#include "Input.h"
#include "Player.h"


//define the vertex structure
struct POS_COL_VERTEX
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 Col;
	DirectX::XMFLOAT2 Texture0;
	DirectX::XMFLOAT3 Normal;
};

//defining the constant buffer structure
struct CONSTANT_BUFFER0
{
	DirectX::XMMATRIX WorldViewProjection;	// 64 bytes
	//float RedAmount;						// 4 bytes
	DirectX::XMVECTOR directional_light_vector; // 16 bytes
	DirectX::XMVECTOR directional_light_colour; // 16 bytes
	DirectX::XMVECTOR ambient_light_colour; // 16 bytes
}; // total size is now 112

//a enum to determine npcs move
enum Node { N1, N2, N3, N4 };

class Render
{
private:

	ID3D11Buffer* VertexBuffer;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;

	ID3D11Buffer* ConstantBuffer0;

	ID3D11SamplerState* Sampler0;
	ID3D11ShaderResourceView* Texture0;

	Camera* TheCamera;
	
	//lighting vectors
	DirectX::XMVECTOR Directional_Light_Shines_From;
	DirectX::XMVECTOR Directional_Light_Colour;
	DirectX::XMVECTOR Ambient_Light_Colour;

	// A Test Model
	Model* TestModel;
	
	// the skybox
	SkyBox* TheSkyBox;

	//the world
	World* TheWorld;

	// the first NPC (move around map)
	NPC* NPCOne;

	// the second npc (follow the player)
	NPC* NPCTwo;

	// the player
	Player* PlayerOne;

	//nodes for the NPC to move around
	Model* Node1;
	Model* Node2;
	Model* Node3;
	Model* Node4;


	// some bools for collisions
	bool TopColliding		= false;
	bool BottomColliding	= false;
	bool LeftColliding		= false;
	bool RightColliding		= false;
	bool BottomCollidingNPC	= false;
	bool TopCollidingNPC	= false;

	bool Rotated = false;

	Node currentnode = N1;

public:

	Render();
	~Render();


	HRESULT IntialiseGraphics(DirectSetUp* SetUp);
	
	void RenderFrame(DirectSetUp* SetUp, Input* theinput);
	
	Camera* ReturnCamera();

	Player* ReturnPlayer();
};

