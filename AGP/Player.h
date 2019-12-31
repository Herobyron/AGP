#pragma once
#include "Entity.h"
class Player : public Entity
{
private:

	// the score for the player when collecting coins
	float Score;



public:

	//a basic constructor
	Player();
	
	// a second constructor that basses itself of the parent constructor
	Player(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name);
	
	// the destructor that uses the parent destructor
	~Player();

	float GetScore();

	void SetScore(float score);
};

