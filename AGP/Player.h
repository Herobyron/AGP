#pragma once
#include "Entity.h"
class Player : public Entity
{
private:

	// the score for the player when collecting coins
	int Score;



public:

	//a basic constructor
	Player();
	
	// a second constructor that basses itself of the parent constructor
	Player(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name);
	
	// the destructor that uses the parent destructor
	~Player();

	// a function to get the players score
	int GetScore();

	// a function to add score to the score variable
	void ADDScore(int score);

	//a function to move the player object down or up depending on the variable put in
	void MoveDown(float distance);
};

