#include "Entity.h"

//entity base constructor
Entity::Entity()
{

}

//entity constructor
Entity::Entity(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name)
{

	//create the model
	TheEntityModel = new Model(device, immediatecontext);

	// setting the models positions and scale
	TheEntityModel->SetY(1);
	TheEntityModel->SetZ(-10);
	TheEntityModel->SetScale(0.5);
	health = 3;

	//load the model in
	TheEntityModel->LoadObjModel(name);

}

//entity destructor
Entity::~Entity()
{
	if (TheEntityModel) delete TheEntityModel;
}

// returns the entity health
float Entity::GetHealth()
{
	return health;
}

//returns the entitys model
Model* Entity::ReturnModel()
{
	return TheEntityModel;
}

// damages the entity with the amount of damage inputed
void Entity::Damage(float damageamount)
{
	health -= damageamount;
}