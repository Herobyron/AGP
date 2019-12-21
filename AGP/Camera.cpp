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

Camera::~Camera()
{

}

void Camera::Rotate(float degrees)
{
	M_Camera_Rotation += degrees;

	//recalculate the dx and dz values
	M_DX = sin(M_Camera_Rotation * DirectX::XM_PI / 180);
	M_DZ = cos(M_Camera_Rotation * DirectX::XM_PI / 180);

}

void Camera::Foward(float distance)
{
	M_X += distance * M_DX;
	M_Z += distance * M_DZ;
}

void Camera::Up(float distance)
{
	M_Y += distance;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{

	M_Position = DirectX::XMVectorSet(M_X, M_Y, M_Z, 0.0);
	M_LookAt = DirectX::XMVectorSet(M_X + M_DX, M_Y, M_Z + M_DZ, 0.0f);
	M_Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(M_Position, M_LookAt, M_Up);

	return view;
}