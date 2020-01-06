#pragma once
#include "Model.h"

// this is the base class for the player and the npc objects
class Entity
{
private:

	// the health of the entity
	float health;

	// the starting positions of the entity
	float PosX, PosY, PosZ;

	// the starting rotations of the entity
	float AngleX, AngleY, AngleZ;

	// the starting scale of the entity
	float Scale;

protected:

	// the model object to display the entity
	Model* TheEntityModel;

public:

	// the entitys basic constructor
	Entity();

	// the entitys second constructor which takes a device, immediate context and name to initialsise the model for the entity
	Entity(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name);

	// the entitys basic destructor
	~Entity();

	// a function to get the entitys health
	float GetHealth();

	// a function to return the model of the entity
	Model* ReturnModel();

	// decrease the health when the entity is hit(this will only apply to the Player)
	void Damage(float damageamount);
};

