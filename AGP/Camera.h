#pragma once
#include <d3d11.h>
#define _XM_NO_INITINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include <math.h>


class Camera
{
private:

	float m_X, m_Y, m_Z;
	float m_DX , m_DZ;
	float m_CameraRotation;

	DirectX::XMVECTOR m_Position;
	DirectX::XMVECTOR m_LookAt;
	DirectX::XMVECTOR m_UP;

public:

	Camera();
	Camera(float init_x, float init_y, float init_z, float init_camera_rotation);
	~Camera();

	void Rotate(float Angle);
	void Foward(float distance);
	void Up(float distance);
	void Strafe(float distance);

	float GetX();
	float GetY();
	float GetZ();

	DirectX::XMMATRIX GetViewMatrix();

};

