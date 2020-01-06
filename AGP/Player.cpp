#include "Player.h"

// player base constructor
Player::Player()
{

}

// player second constructor takes a device and immediate context refernce and a name and uses the entity constructor to initialise it
Player::Player(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name)
	:Entity{device, immediatecontext, name}
{
	
}

// player destructor that uses the entity destructor
Player::~Player()
{
	Entity::~Entity();
}

//returns the players score
int Player::GetScore()
{
	return Score;
}

// adds score to the players existing score count
void Player::ADDScore(int score)
{
	Score += score;
}

//a function to move the player model down
void Player::MoveDown(float distance)
{
	TheEntityModel->ChangeYPos(distance);
}