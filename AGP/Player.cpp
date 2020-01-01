#include "Player.h"

Player::Player()
{

}

Player::Player(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name)
	:Entity{device, immediatecontext, name}
{
	
}

Player::~Player()
{
	Entity::~Entity();
}

int Player::GetScore()
{
	return Score;
}

void Player::ADDScore(int score)
{
	Score += score;
}

void Player::MoveDown(float distance)
{
	TheEntityModel->ChangeYPos(distance);
}