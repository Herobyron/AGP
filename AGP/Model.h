#pragma once
#include "objfilemodel.h"
#include <directxmath.h>

struct MODEL_CONSTANT_BUFFER
{
	DirectX::XMMATRIX WorldViewProjection;	// 64 bytes
};


class Model
{
private:

	//refernce to the device and immediate context created in setup.
	ID3D11Device* ModelDevice;
	ID3D11DeviceContext* ModelImmediateContext;

	//creates a refernce of ObjFileModel
	ObjFileModel* ModelObject;

	// the models vertex shader
	ID3D11VertexShader* ModelVShader;

	// the models pixel shader
	ID3D11PixelShader* ModelPShader;

	//the models input layout
	ID3D11InputLayout* ModelInputLayout;

	// the models constant buffer
	ID3D11Buffer* ModelConstantBuffer;

	// the models x, y, z position
	float ModelX, ModelY, ModelZ;

	// the models angles on the x, y and z axis
	float ModelAngleX, ModelAngleY, ModelAngleZ;

	// the models scale
	float ModelScale;



	//bounding sphere information used for collision
	float BoundingSphereCentreX;
	float BoundingSphereCentreY;
	float BoundingSphereCentreZ;

	float BoungingSphereRadius;


	//try adding something to change the texture of each of the models here
	// (do once tutorials complete)

public:

	//model constructor
	// the constructor takes two parametes. a refernce to the device
	// and a refernce to the immediate context which were created in setup
	Model(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

	//model destructor
	~Model();

	// a function to orientate a model to be facing a certain position
	//parameters, the x and z positions in world space
	void LookAt_XZ(float WorldSpaceX, float WorldSpaceZ);

	// a function to move the model foward 
	//parameter is the distance at which you want the model to move foward by
	void MoveFoward(float distance);

	// a function to move the model to the left and to the right
	// takes a parameter for the distance the model should move
	void MoveSideWays(float distance);

	//a function that loads the object into the game
	HRESULT LoadObjModel(char* FileName);

	// a function to actually draw the model to the screen
	void Draw(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	//functions to set all of the positions of the model
	void SetX(float X);
	void SetY(float Y);
	void SetZ(float Z);

	//functions to set all of the rotations of the model
	void SetXAngle(float X);
	void SetYAngle(float Y);
	void SetZAngle(float Z);

	// a function to set the scale of the object
	void SetScale(float scale);

	//functions to get all of the positions of the model
	float ReturnXPos();
	float ReturnYPos();
	float ReturnZPos();

	//functions of get all of the rotations of the model
	float ReturnXAngle();
	float ReturnYAngle();
	float ReturnZAngle();

	// a function to get the scale of the model
	float ReturnScale();

	// functions to change the models positions
	void ChangeXPos(float ChangeAmount);
	void ChangeYPos(float ChangeAmount);
	void ChangeZPos(float ChangeAmount);

	//functions to change the models rotations
	void ChangeXAngle(float ChangeAmount);
	void ChangeYAngle(float ChangeAmount);
	void ChangeZAngle(float ChangeAmount);

	//a function to change the models scale
	void ChangeScale(float ChangeAmount);


	// a function to calculate the model centre point
	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();

	float GetBoundingSphereRadius();
	DirectX::XMVECTOR GetBoundingSphereWorldSpacePosition();
	bool CheckCollision(Model* OtherModel);
	bool CheckCollisionAABB(Model* OtherModel);


};

