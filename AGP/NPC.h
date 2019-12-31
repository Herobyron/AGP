#pragma once
#include "Entity.h"
class NPC : public Entity
{
private:


public:

	//basic constructor
	NPC();

	// a second constructor that bases itself off the parent constructor
	NPC(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name);

	// destructor
	~NPC();

	// a function to move the NPC
	void Movement();

	//a function to set the npc on the ground
	void MoveDown(float distance);

};

