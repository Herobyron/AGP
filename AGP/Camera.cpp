#include "Camera.h"

//default constructor
Camera::Camera()
{

}

//constructor that takes all xyz positions of camera and the intitial rotation
Camera::Camera(float x, float y, float z, float rotation)
{
	M_X = x;
	M_Y = y;
	M_Z = z;
	M_Camera_Rotation = rotation;

	M_DX = sin(M_Camera_Rotation * DirectX::XM_PI / 180);
	M_DZ = cos(M_Camera_Rotation * DirectX::XM_PI / 180);
}

// destructor
Camera::~Camera()
{

}

// a function used to rotate the camera by the amount of degrees inputed into the variable
void Camera::Rotate(float degrees)
{
	// increase the camera location
	M_Camera_Rotation += degrees;

	//recalculate the dx and dz values
	M_DX = sin(M_Camera_Rotation * DirectX::XM_PI / 180);
	M_DZ = cos(M_Camera_Rotation * DirectX::XM_PI / 180);

}

// a function to move the camera foward by the input of the vriable
void Camera::Foward(float distance)
{
	// increases dx and dz by the distance times the previous value
	M_X += distance * M_DX;
	M_Z += distance * M_DZ;
}

// moves the camera up depending on the distance inputed into the function
void Camera::Up(float distance)
{
	M_Y += distance;
}

// returns the cameras view matrix
DirectX::XMMATRIX Camera::GetViewMatrix()
{
	//calculates the cameras position, lookat and up position
	M_Position = DirectX::XMVectorSet(M_X, M_Y, M_Z, 0.0);
	M_LookAt = DirectX::XMVectorSet(M_X + M_DX, M_Y, M_Z + M_DZ, 0.0f);
	M_Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// calculates the view matrix
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(M_Position, M_LookAt, M_Up);

	//return the view 
	return view;
}


float Camera::GetX()
{
	return M_X;
}

float Camera::GetY()
{
	return M_Y;
}

float Camera::GetZ()
{
	return M_Z;
}