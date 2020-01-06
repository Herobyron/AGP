#include "World.h"

// world constructor
World::World()
{   //pushing back the floor strings
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");
	Layoutfloor.push_back("BBBBBBBBBBBBBB");

	// pushing back the coin strings
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");
	LayoutCoin.push_back("....C....C....");
	LayoutCoin.push_back("C...C....C...C");

	// pushes back the walls layout
	LayoutWalls.push_back("WWWWWWWWWWWWWWWW");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("W..............W");
	LayoutWalls.push_back("WWWWWWWWWWWWWWWW");

	FloorScale = 1.0f;
}

// the world destructor
World::~World()
{
	WorldBlocks.clear();
	Layoutfloor.clear();
	Coins.clear();
	LayoutCoin.clear();
}

// gets the floors hieght
float World::GetFloorHieght()
{
	return Layoutfloor.size();
}

// gets the floors width
float World::GetFloorWidth()
{
	return Layoutfloor[0].length();
}

// gets the coins hieght
float World::GetCoinHieght()
{
	return LayoutCoin.size();
}

// get coins width
float World::GetCoinWidth()
{
	return LayoutCoin[0].length();
}

//gets the wall hieght
float World::GetWallHieght()
{
	return LayoutWalls.size();
}

//get wall width
float World::GetWallWidth()
{
	return LayoutWalls[0].length();
}

// initialsies the floor with the device and the immediate context
void World::InitialiseFloor(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
	//uses for loops to iterate throught the vector of values
	for (int h = 0; h < GetFloorHieght(); h++)
	{
		for (int r = 0; r < GetFloorWidth(); r++)
		{
			float xpos = r;
			float zpos = h;

			switch (Layoutfloor[h][r])
			{
				case('B'):
				{
					// this will just be a standard block 
					Model* FloorBlock = new Model(device, immediatecontext);

					

					//FloorBlock->SetScale(FloorScale);
					//sets up the floor and pushes it back to the vector
					FloorBlock->SetX((r * 2) - 12);
					FloorBlock->SetY(-2);
					FloorBlock->SetZ((h * 2) - 10);
					FloorBlock->SetScale(1);
					FloorBlock->LoadObjModel((char*)"assets/cube.obj");

					WorldBlocks.push_back(FloorBlock);
					break;
				}
				case('.'):
				{
					break;
				}
			}
		}
	}
}

// intiaislise the walls with the device and immediate context
void World::InitialiseWalls(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
	//uses for loops to iterate throught the vector of values
	for (int h = 0; h < GetWallHieght(); h++)
	{
		for (int r = 0; r < GetWallWidth(); r++)
		{
			float xpos = r;
			float zpos = h;

			switch (LayoutWalls[h][r])
			{
			case('W'):
			{
				// this will just be a standard block 
				Model* WallBlock = new Model(device, immediatecontext);



				//FloorBlock->SetScale(FloorScale);
				//sets up the wall an pushes it back into the vector
				WallBlock->SetX((r * 2) - 14);
				WallBlock->SetY(0);
				WallBlock->SetZ((h * 2) - 12);

				WallBlock->SetScale(1.0f);

				WallBlock->LoadObjModel((char*)"assets/cube.obj");

				WorldWalls.push_back(WallBlock);
				break;
			}
			case('.'):
			{
				break;
			}
			}
		}
	}
}

//initialises the coins using the device and immediate context 
void World::InitialiseCoins(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
	//uses for loops to iterate throught the vector of values
	for (int h = 0; h < GetCoinHieght(); h++)
	{
		for (int r = 0; r < GetCoinWidth(); r++)
		{
			float xpos = r;
			float zpos = h;

			switch (LayoutCoin[h][r])
			{
				case('C'):
				{
					// this will just be a standard block 
					Model* CoinBlock = new Model(device, immediatecontext);



					//FloorBlock->SetScale(FloorScale);
					//sets up the coin and pushes it back into the vector
					CoinBlock->SetX((r * 2) - 12);
					CoinBlock->SetY(0);
					CoinBlock->SetZ((h * 2) - 10);

					CoinBlock->SetScale(0.01f);

					CoinBlock->LoadObjModel((char*)"assets/COIN.obj");

					Coins.push_back(CoinBlock);
					break;
				}
				case('.'):
				{
					break;
				}
			}
		}
	}
}

// function to draw all the floors in the vector
void World::DrawFloor(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	for (int i = 0; i < WorldBlocks.size(); i++)
	{

		WorldBlocks[i]->Draw(view, projection);
	}
}

// function to draw all the coins in the vector
void World::DrawCoin(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	for (int i = 0; i < Coins.size(); i++)
	{
		Coins[i]->Draw(view, projection);
	}
}

// draws the walls in the vector
void World::DrawWalls(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	for (int i = 0; i < WorldWalls.size(); i++)
	{
		WorldWalls[i]->Draw(view, projection);
	}
}

//tests all the colliisnos of the floors in the vector
bool World::TestFloorCollision(Model* OtherModel)
{
	// testing for collision with all blocks stored in the world vector
	for (int i = 0; i < WorldBlocks.size(); i++)
	{
		if (WorldBlocks[i]->CheckCollision(OtherModel))
		{
			// if they are colliding then return true to show collision
			return true;
		}
	}

	// if none of the above are colliding then return false for no collision
	return false;

}

//tests all the coins collsions within the vector
bool World::TestCoinCollision(Model* OtherModel)
{
	// testing for collision with all blocks stored in the world vector
	for (int i = 0; i < Coins.size(); i++)
	{
		if (Coins[i]->CheckCollision(OtherModel))
		{
			// set the coin to move
			Coins[i]->SetY(1000);
			return true;
		}
	}

	return false;



}

// tests the wall collsions of all of them in the vector
bool World::TestWallCollision(Model* OtherModel)
{
	// testing for collision with all blocks stored in the world vector
	for (int i = 0; i < WorldWalls.size(); i++)
	{
		if (WorldWalls[i]->CheckCollision(OtherModel))
		{
			
			return true;
		}
	}

	return false;
}