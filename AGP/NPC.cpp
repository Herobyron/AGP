#include "NPC.h"

//base constructor
NPC::NPC()
{

}

// second constructor which uses the constructor of the entity
NPC::NPC(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name)
	:Entity{device, immediatecontext, name}
{

}

//destructor that calls the npc destructor
NPC::~NPC()
{
	Entity::~Entity();
}

void NPC::Movement()
{

}

// a function to move the npc down
void NPC::MoveDown(float distance)
{
	TheEntityModel->ChangeYPos(distance);
}