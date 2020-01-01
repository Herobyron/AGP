#include "Render.h"

//
Render::Render()
{
	//creating the camera
	TheCamera = new Camera(0.0f, 0.0f, -11.0f, 0.0f);
	
	//creating the world
	TheWorld = new World();



}


//
Render::~Render()
{
	if (VertexBuffer) VertexBuffer->Release();
	if (InputLayout) InputLayout->Release();
	if (VertexShader) VertexShader->Release();
	if (PixelShader) PixelShader->Release();
	if (ConstantBuffer0) ConstantBuffer0->Release();
	if (Sampler0) Sampler0->Release();
	if (Texture0) Texture0->Release();
	if (TheCamera) delete TheCamera;
}


//the render frame
void Render::RenderFrame(DirectSetUp* SetUp, Input* theinput)
{

	//clear the back buffer 
	float rgba_clear_color[4] = { 0.3f, 0.3f, 0.7f, 1.0f };
	SetUp->ReturnImmediateContext()->ClearRenderTargetView(SetUp->ReturnBackBufferView(), rgba_clear_color);
	SetUp->ReturnImmediateContext()->ClearDepthStencilView(SetUp->ReturnZBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//setting the lighting colour (Make something to update light colour when needed)
	Directional_Light_Shines_From = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	Directional_Light_Colour = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // green
	Ambient_Light_Colour = DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f); // dark grey

	//render here
	//set the vertex buffer
	UINT stride = sizeof(POS_COL_VERTEX);
	UINT offset = 0;
	SetUp->ReturnImmediateContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	
	//selectwhich primtive to use
	SetUp->ReturnImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//updating the constant buffer
	DirectX::XMMATRIX transpose;
	CONSTANT_BUFFER0 CB0_Values;

	//updating the world view projection matrix
	DirectX::XMMATRIX projection, world, view, scale, rotate, translate;

	//remember the multiplication is scale then rotate then translate
	scale = DirectX::XMMatrixScaling(1, 1, 1);
	rotate = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(45));
	translate = DirectX::XMMatrixTranslation(0, 0, 5);

	world = scale * rotate * translate;

	transpose = DirectX::XMMatrixTranspose(world);

	// stuff for lighting
	CB0_Values.directional_light_colour = Directional_Light_Colour;
	CB0_Values.ambient_light_colour = Ambient_Light_Colour;
	CB0_Values.directional_light_vector = DirectX::XMVector3Transform(Directional_Light_Shines_From, transpose);
	CB0_Values.directional_light_vector = DirectX::XMVector3Normalize(CB0_Values.directional_light_vector);

	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0), 640.0 / 480.0, 1.0, 100.0);
	view = TheCamera->GetViewMatrix();
	CB0_Values.WorldViewProjection = world * view * projection;


	//upload the new values for the constant buffer
	SetUp->ReturnImmediateContext()->UpdateSubresource(ConstantBuffer0, 0, 0, &CB0_Values, 0, 0);

	//seting the constant buffer to be active
	SetUp->ReturnImmediateContext()->VSSetConstantBuffers(0, 1, &ConstantBuffer0);

	//apply the textures just before the object has been drawn
	// first number for each is what number texture this is in index (so zero as there is only one)
	// and the secon parameters is the amount of them bieng set (one for now) 
	SetUp->ReturnImmediateContext()->PSSetSamplers(0, 1, &Sampler0);
	SetUp->ReturnImmediateContext()->PSSetShaderResources(0, 1, &Texture0);

	//text seems to remove every other object except itself
	//text (this is in the top corner of the window (for now))
	

	//drawing the world
	TheWorld->DrawFloor(view, projection);
	TheWorld->DrawCoin(view, projection);
	TheWorld->DrawWalls(view, projection);

	//drawing the model and setting its position
	//TestModel->Draw(view, projection);

	Node1->Draw(view, projection);
	Node2->Draw(view, projection);
	Node3->Draw(view, projection);
	Node4->Draw(view, projection);

	// skybox stuff
	TheSkyBox->Draw(view, projection);

	//stuff for the npc and its movement
	//this should test to see if npc one is colliding with anything its it isnt then it should move downwards
	
	if (theinput->ISKeyPressed(DIK_SPACE))
	{
		
		if (!TheWorld->TestFloorCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->MoveDown(1 * 0.01);
			TopColliding = false;
		}
		else
		{
			TopColliding = true;
		}
	}
	else
	{
		if (!TheWorld->TestFloorCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->MoveDown(-1 * 0.01);
			BottomColliding = false;
		}
		else
		{
			BottomColliding = true;
		}
	}

	if (theinput->ISKeyPressed(DIK_UP))
	{
		PlayerOne->ReturnModel()->MoveFoward(1.0 * 0.01);
		if (TheWorld->TestCoinCollision(PlayerOne->ReturnModel()))
		{
			// add score
			PlayerOne->ADDScore(100);
		}
		if (TheWorld->TestWallCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->ReturnModel()->ChangeZPos(-1);
		}
	}
	else if (theinput->ISKeyPressed(DIK_DOWN))
	{
		PlayerOne->ReturnModel()->MoveFoward(-1.0 * 0.01);
		if (TheWorld->TestCoinCollision(PlayerOne->ReturnModel()))
		{
			// add score
			PlayerOne->ADDScore(100);
		}
		if (TheWorld->TestWallCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->ReturnModel()->ChangeZPos(1);
		}
	}
	else if (theinput->ISKeyPressed(DIK_LEFT))
	{
		PlayerOne->ReturnModel()->MoveSideWays(-1.0 * 0.01);
		if (TheWorld->TestCoinCollision(PlayerOne->ReturnModel()))
		{
			// add score
			PlayerOne->ADDScore(100);
		}
		if (TheWorld->TestWallCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->ReturnModel()->ChangeXPos(1);
		}
	}
	else if (theinput->ISKeyPressed(DIK_RIGHT))
	{
		PlayerOne->ReturnModel()->MoveSideWays(1.0 * 0.01);
		if (TheWorld->TestCoinCollision(PlayerOne->ReturnModel()))
		{
			// add score
			PlayerOne->ADDScore(100);
		}
		if (TheWorld->TestWallCollision(PlayerOne->ReturnModel()))
		{
			PlayerOne->ReturnModel()->ChangeXPos(-1);
		}
	}
	


	//checking if the player is colliding with floor and pushing the model up to make sure they dont fall through and can move
	if (BottomColliding)
	{
		PlayerOne->MoveDown(1 * 0.1);
		BottomColliding = false;
	}
	else if (TopColliding)
	{
		PlayerOne->MoveDown(-1 * 0.1);
		TopColliding = false;
	}

	// npc movement stuff

	// simulate gravity
	//if (!TheWorld->TestFloorCollision(NPCOne->ReturnModel()))
	//{
	//	NPCOne->MoveDown(-1 * 0.01);
	//	BottomCollidingNPC = false;
	//}
	//else
	//{
	//	BottomCollidingNPC = true;
	//}
	//
	//if (BottomCollidingNPC)
	//{
	//	NPCOne->MoveDown(1 * 0.1);
	//	BottomCollidingNPC = false;
	//}
	

	// the other movement (movement for npc based off of nodes)
	switch (currentnode)
	{
		case(N1):
		{
			if (!Rotated)
			{
				NPCOne->ReturnModel()->SetYAngle(270);
				NPCOne->ReturnModel()->SetZ(-10);
				Rotated = true;
			}
			break;
		}
		case(N2):
		{
			if (!Rotated)
			{
				NPCOne->ReturnModel()->SetYAngle(180);
				NPCOne->ReturnModel()->SetX(10);
				Rotated = true;
			}
			break;
		}
		case(N3):
		{
			if (!Rotated)
			{
				NPCOne->ReturnModel()->SetYAngle(90);
				NPCOne->ReturnModel()->SetZ(10);
				Rotated = true;
			}
			break;
		}
		case(N4):
		{
			if (!Rotated)
			{
				NPCOne->ReturnModel()->SetYAngle(0);
				NPCOne->ReturnModel()->SetX(Node3->ReturnXPos());
				Rotated = true;
			}
			break;
		}
		default:
			break;
	}

	// npc actually moving
	NPCOne->ReturnModel()->MoveFoward(1 * 0.01);

	// test the collisions
	if (NPCOne->ReturnModel()->CheckCollision(Node1))
	{
		currentnode = N2;
		Rotated = false;
	}
	else if (NPCOne->ReturnModel()->CheckCollision(Node2))
	{
		currentnode = N3;
		Rotated = false;
	}
	else if (NPCOne->ReturnModel()->CheckCollision(Node3))
	{
		currentnode = N4;
		Rotated = false;
	}
	else if (NPCOne->ReturnModel()->CheckCollision(Node4))
	{
		currentnode = N1;
		Rotated = false;
	}

	SetUp->ReturnImmediateContext()->PSSetSamplers(0, 1, &Sampler0);
	SetUp->ReturnImmediateContext()->PSSetShaderResources(0, 1, &Texture0);

	NPCOne->ReturnModel()->Draw(view, projection);

	PlayerOne->ReturnModel()->Draw(view, projection);


	SetUp->ReturnText()->AddText( "score : " + to_string(PlayerOne->GetScore()), -1, 1, 0.1);
	SetUp->ReturnText()->RenderText();

	//draw the vertex buffer to the back buffer
	SetUp->ReturnImmediateContext()->Draw(36, 0);

	//display what was rendered
	SetUp->ReturnSwapChain()->Present(0, 0);

}

HRESULT Render::IntialiseGraphics(DirectSetUp* SetUp)
{
	HRESULT hr = S_OK;
	// setting up the test model
	TestModel = new Model(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	TestModel->SetX(3);
	TestModel->SetY(3);
	TestModel->SetZ(0);
	TestModel->SetYAngle(0);
	TestModel->SetScale(1);
	TestModel->LoadObjModel((char*)"assets/Sphere.obj");

	//setting up the nodes
	Node1 = new Model(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	Node1->SetX(10);
	Node1->SetY(1);
	Node1->SetZ(10);
	Node1->SetScale(1);
	Node1->LoadObjModel((char*)"assets/cube.obj");


	Node2 = new Model(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	Node2->SetX(-10);
	Node2->SetY(1);
	Node2->SetZ(10);
	Node2->SetScale(1);
	Node2->LoadObjModel((char*)"assets/cube.obj");

	Node3 = new Model(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	Node3->SetX(-10);
	Node3->SetY(1);
	Node3->SetZ(-10);
	Node3->SetScale(1);
	Node3->LoadObjModel((char*)"assets/cube.obj");


	Node4 = new Model(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	Node4->SetX(10);
	Node4->SetY(1);
	Node4->SetZ(-10);
	Node4->SetScale(1);
	Node4->LoadObjModel((char*)"assets/cube.obj");



	//setting up the skybox
	TheSkyBox = new SkyBox(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());
	TheSkyBox->SetBoxX(0);
	TheSkyBox->SetBoxY(0);
	TheSkyBox->SetBoxZ(5);
	TheSkyBox->SetBoxScale(20);
	TheSkyBox->SkyBoxInitialisation();
	
	//setting up the world and creating it
	TheWorld->InitialiseFloor(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());

	//setting up the coins and creating it
	TheWorld->InitialiseCoins(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());

	//setting up the walls
	TheWorld->InitialiseWalls(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext());

	//creating the npc
	NPCOne = new NPC(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext(), (char*)"assets/cube.obj");

	//creating the player
	PlayerOne = new Player(SetUp->ReturnDevice(), SetUp->ReturnImmediateContext(), (char*)"assets/Sphere.obj");
	PlayerOne->ReturnModel()->SetScale(0.5f);
	PlayerOne->ReturnModel()->SetY(0);
	PlayerOne->ReturnModel()->SetZ(0);


	//define verticies of a triangle - screen coordinates -1 to +1
	POS_COL_VERTEX verticies[] =
	{
		//back face
		{DirectX::XMFLOAT3(-1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)},

		//front face
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)},

		//left face
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)},

		//right face
		{DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},

		//bottom face
		{DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f),	DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f),	DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f),	DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f),	DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.3f, 0.6f, 0.9f, 1.0f),	DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)},

		//top face
		{DirectX::XMFLOAT3( 1.0f, 1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 1.0f, 1.0f,  1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},

	};

	//creating the texture
	D3DX11CreateShaderResourceViewFromFile(SetUp->ReturnDevice(), "assets/texture.bmp", NULL, NULL, &Texture0, NULL);

	//set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(verticies);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//creating the buffer
	hr = SetUp->ReturnDevice()->CreateBuffer(&bufferDesc, NULL, &VertexBuffer);

	//cehck to see if failed and return if it did
	if (FAILED(hr))
	{
		return hr;
	}

	//setting up and creating the constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //can be updated with update subresource
	constant_buffer_desc.ByteWidth = 112; // must be a multiple of 16. this number is calculated from the constant buffer structure
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //use as a constant

	hr = SetUp->ReturnDevice()->CreateBuffer(&constant_buffer_desc, NULL, &ConstantBuffer0);

	if (FAILED(hr))
	{
		return hr;
	}



	//copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//lock the buffer to allow for writing
	SetUp->ReturnImmediateContext()->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//copy the data
	memcpy(ms.pData, verticies, sizeof(verticies));

	//unlock the buffer
	SetUp->ReturnImmediateContext()->Unmap(VertexBuffer, NULL);

	//load and compile the pixel and vertex shader (using vs_5_0 to target DX11 hardware only)
	ID3DBlob* VS, *PS, *Error;

	//finding the vertex shader
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &Error, 0);

	if (Error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
			return hr;
	}

	//finding the pixel shader
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &Error, 0);

	if (Error != 0)//check for shader compilation error
	{
		OutputDebugStringA((char*)Error->GetBufferPointer());
		Error->Release();
		if (FAILED(hr))
			return hr;
	}


	//create the shader objects
	hr = SetUp->ReturnDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VertexShader);

	if (FAILED(hr))
		return hr;

	hr = SetUp->ReturnDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);

	if (FAILED(hr))
		return hr;

	//set the shader objects as active
	SetUp->ReturnImmediateContext()->VSSetShader(VertexShader, 0, 0);
	SetUp->ReturnImmediateContext()->PSSetShader(PixelShader, 0, 0);

	//create and set the input layout
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//make sure they are correct dxgi format and d3d versions
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = SetUp->ReturnDevice()->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &InputLayout);

	if (FAILED(hr))
		return hr;

	SetUp->ReturnImmediateContext()->IASetInputLayout(InputLayout);

	//creating the Sampler
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	SetUp->ReturnDevice()->CreateSamplerState(&sampler_desc, &Sampler0);

	

	return S_OK;

}

Camera* Render::ReturnCamera()
{
	return TheCamera;
}

Player* Render::ReturnPlayer()
{
	return PlayerOne;
}