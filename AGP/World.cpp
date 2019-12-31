#include "World.h"

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


	FloorScale = 1.0f;
}

World::~World()
{
	WorldBlocks.clear();
	Layoutfloor.clear();
	Coins.clear();
	LayoutCoin.clear();
}

float World::GetFloorHieght()
{
	return Layoutfloor.size();
}

float World::GetFloorWidth()
{
	return Layoutfloor[0].length();
}

float World::GetCoinHieght()
{
	return LayoutCoin.size();
}

float World::GetCoinWidth()
{
	return LayoutCoin[0].length();
}


void World::InitialiseFloor(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{

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

void World::InitialiseCoins(ID3D11Device* device, ID3D11DeviceContext* immediatecontext)
{
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

void World::DrawFloor(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	for (int i = 0; i < WorldBlocks.size(); i++)
	{

		WorldBlocks[i]->Draw(view, projection);
	}
}

void World::DrawCoin(DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	for (int i = 0; i < Coins.size(); i++)
	{
		Coins[i]->Draw(view, projection);
	}
}

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

bool World::TestCoinCollision(Model* OtherModel)
{
	// testing for collision with all blocks stored in the world vector
	for (int i = 0; i < Coins.size(); i++)
	{
		if (Coins[i]->CheckCollision(OtherModel))
		{
			return true;
		}
	}

	return false;



}