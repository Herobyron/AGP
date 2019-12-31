#include "Entity.h"

Entity::Entity()
{

}

Entity::Entity(ID3D11Device* device, ID3D11DeviceContext* immediatecontext, char* name)
{

	//create the model
	TheEntityModel = new Model(device, immediatecontext);


	TheEntityModel->SetY(10);
	TheEntityModel->SetZ(-2);
	TheEntityModel->SetScale(0.5);

	//load the model in
	TheEntityModel->LoadObjModel(name);

}

Entity::~Entity()
{
	if (TheEntityModel) delete TheEntityModel;
}

float Entity::GetHealth()
{
	return health;
}

Model* Entity::ReturnModel()
{
	return TheEntityModel;
}