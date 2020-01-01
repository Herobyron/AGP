#pragma once
#include "Model.h"
#include <vector>

class World
{
private:

	//a vector to hold all of the models for the world 
	std::vector<Model*> WorldBlocks;

	//a vector of string that holds all of the placements of the floor
	std::vector<std::string> Layoutfloor;

	//a vector that holds the coins that will be included within the level
	std::vector<Model*> Coins;

	// a vector that stores the placements of all of the coins in the world 
	std::vector<std::string> LayoutCoin;

	// a vector that stores the placements of all the walls in the world
	std::vector<std::string> LayoutWalls;

	// a vector that stores all of the wall blocks
	std::vector<Model*> WorldWalls;

	//floats to determine the floor scale
	float FloorScale;

public:

	World();
	~World();

	// a function to initialise the floor and put all block into vectors ready to be drawn
	void InitialiseFloor(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

	// a function to initialise the coins and put all of them into the correct vector ready to be drawn
	void InitialiseCoins(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

	// a function to initialise the walls and put all of them into the correct vector ready for drawing
	void InitialiseWalls(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

	// a function to go through the floor vector to draw 
	void DrawFloor(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	//a function to go through the coin vector and draw 
	void DrawCoin(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	// a function to go through the wall vector and draw
	void DrawWalls(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	float GetFloorHieght();
	float GetFloorWidth();

	float GetCoinHieght();
	float GetCoinWidth();

	float GetWallHieght();
	float GetWallWidth();

	bool TestFloorCollision(Model* OtherModel);
	bool TestCoinCollision(Model* OtherModel);
	bool TestWallCollision(Model* OtherModel);
};

