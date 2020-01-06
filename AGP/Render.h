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
#include "ParticleGenerator.h"

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

	// the renderers vertex buffer
	ID3D11Buffer* VertexBuffer;
	
	// the pixel and vertex shader used within the renderer
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	
	// the input layout of the renderer
	ID3D11InputLayout* InputLayout;

	// refernce to renderers constant buffer
	ID3D11Buffer* ConstantBuffer0;

	// sampler and texture of the renderer
	ID3D11SamplerState* Sampler0;
	ID3D11ShaderResourceView* Texture0;

	// a pointer to the camera class
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

	// the particle generator
	ParticleGenerator* TheGenerator;
	// a float for the camera position
	DirectX::XMFLOAT3 CameraPos;


	// some bools for collisions
	bool TopColliding		= false;
	bool BottomColliding	= false;
	bool LeftColliding		= false;
	bool RightColliding		= false;
	bool BottomCollidingNPC	= false;
	bool TopCollidingNPC	= false;

	// a bool to see if its rotated
	bool Rotated = false;

	// the node that the first NPC is using
	Node currentnode = N1;

public:

	// the besic constructor of the renderer
	Render();

	// the basic destructor of the renderer
	~Render();

	//a function that takes a pointer to the direct set up class. and initialises graphics
	HRESULT IntialiseGraphics(DirectSetUp* SetUp);
	
	// a function that renders everything in the scene
	void RenderFrame(DirectSetUp* SetUp, Input* theinput);
	
	// returns a pointer to the camera
	Camera* ReturnCamera();

	// returns a pointer to the player
	Player* ReturnPlayer();
};

