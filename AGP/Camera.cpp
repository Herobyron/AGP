#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(float init_x, float init_y, float init_z, float init_camera_rotation)
	:m_X{ init_x }, m_Y{ init_y }, m_Z{ init_z }, m_CameraRotation{ init_camera_rotation }, m_DX{ 0 }, m_DZ{ 0 }
{
	m_DX = sin(m_CameraRotation * (DirectX::XM_PI / 180));
	m_DZ = cos(m_CameraRotation * (DirectX::XM_PI / 180));
}


Camera::~Camera()
{

}

void Camera::Rotate(float angle)
{
	m_CameraRotation += angle;

	m_DX = sin(m_CameraRotation * (DirectX::XM_PI / 180));
	m_DZ = cos(m_CameraRotation * (DirectX::XM_PI / 180));
}

void Camera::Foward(float distance)
{
	m_X = (m_DX * distance);
	m_Z = (m_DZ * distance);
}

void Camera::Up(float distance)
{
	m_Y += distance;
}

void Camera::Strafe(float distance)
{
	DirectX::XMVECTOR temp = DirectX::XMVector3Cross(m_Position, m_LookAt);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	m_Position = DirectX::XMVectorSet(m_X, m_Y, m_Z, 0.0);
	m_LookAt = DirectX::XMVectorSet(m_X + m_DX, m_Y, m_Z + m_DZ, 0.0);
	m_UP = DirectX::XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return DirectX::XMMatrixLookAtLH(m_Position, m_LookAt, m_UP);

}

float Camera::GetX()
{
	return m_X;
}

float Camera::GetY()
{
	return m_Y;
}

float Camera::GetZ()
{
	return m_Z;
}