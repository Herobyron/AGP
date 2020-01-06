#pragma once
#include <d3d11.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include <directxmath.h>


class Camera
{
private:

	// the cameras x,y,z position and the DX and DZ of the camera
	float M_X, M_Y, M_Z, M_DX, M_DZ;

	// the cameras current rotation
	float M_Camera_Rotation;
	
	// the camersa position vector, look at vector, and up vector
	DirectX::XMVECTOR M_Position, M_LookAt, M_Up;



public:
	
	//cameras basic constructor
	Camera();

	//cameras constructor that takes its starting position and its rotation
	Camera(float x, float y, float z, float rotation);
	
	// cameras destructor 
	~Camera();

	//rotate function that takes a value to tell it how much to rotate by
	void Rotate(float degrees);

	//moves the camera Foward by the distance given in the parameters
	void Foward(float distance);

	//moves the camera up or down depending on the distance in the parameter
	void Up(float distance);

	// a function to retern the view matrix of the camera
	DirectX::XMMATRIX GetViewMatrix();

	//functions to get the cameras X, Y and Z positions
	float GetX();
	float GetY();
	float GetZ();

	

};

