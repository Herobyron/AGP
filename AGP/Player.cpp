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

float Player::GetScore()
{
	return Score;
}

void Player::SetScore(float score)
{
	Score = score;
}