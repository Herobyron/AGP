#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <directxmath.h>
#include <directxmath.h>
#include <list>
#include <windows.h>
// this is to supress one of the librarys so that their would be no infinte loops
#pragma comment( lib, "winmm.lib")

// particles constant buffer
struct PARTICLE_CONSTANT_BUFFER
{
	DirectX::XMMATRIX WorldViewProjection;	// 64 bytes
	DirectX::XMFLOAT4 Color; // 16 bytes

};

// a structure for a basic particle
struct Particle
{
	float gravity;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT4 colour;
	float age;
};

// an enum for the type of particle
enum type {RAINBOW_FOUNTAIN};

class ParticleGenerator
{



private:

		//refernce to the device and immediate context created in setup.
		ID3D11Device* ParticleDevice;
		ID3D11DeviceContext* ParticleImmediateContext;


		// the particles vertex shader
		ID3D11VertexShader* ParticleVShader;

		// the particles pixel shader
		ID3D11PixelShader* ParticlePShader;

		//the particles input layout
		ID3D11InputLayout* ParticleInputLayout;

		// the particles constant buffer
		ID3D11Buffer* ParticleConstantBuffer;

		// the particles x, y, z position
		float ParticleX, ParticleY, ParticleZ;

		// the particles angles on the x, y and z axis
		float ParticleAngleX, ParticleAngleY, ParticleAngleZ;

		// the particles scale
		float ParticleScale;

		//the rasterizer state for the particles
		ID3D11RasterizerState* RasterSolid = 0;
		ID3D11RasterizerState* RasterParticle = 0;
		ID3D11DepthStencilState* DepthWriteSolid = 0;
		ID3D11DepthStencilState* DepthWriteParticle = 0;

		// the sampler and texture for the particle
		ID3D11SamplerState*			ParticleSampler;
		ID3D11ShaderResourceView*	ParticleTexture;
		ID3D11Buffer*				ParticleVertexBuffer;

		// the constant buffer for the particle
		PARTICLE_CONSTANT_BUFFER ConstantBufferValues;


		// variables used for creating particles and makeing the generator
		float TimePrevious;
		float UntilParticle;
		float RandomZeroToOne();
		float RandomNegOneToPosOne();

		// the lists for the particles (linked lists)
		std::list<Particle*> Free;
		std::list<Particle*> active;

		std::list<Particle*>::iterator it;

		//a  bool to turn on and off the particle engine
		bool Active = true;

		//enum to use for switch in particle draw
		type TYPE;

public:

		//particle constructor
		// the constructor takes two parametes. a refernce to the device
		// and a refernce to the immediate context which were created in setup
		ParticleGenerator(ID3D11Device* device, ID3D11DeviceContext* immediatecontext);

		//particle destructor
		~ParticleGenerator();

		// a function to orientate a particles to be facing a certain position
		//parameters, the x and z positions in world space
		void LookAt_XZ(float WorldSpaceX, float WorldSpaceZ);

		// a function to move the particle foward 
		//parameter is the distance at which you want the model to move foward by
		void MoveFoward(float distance);

		// a function to move the model to the left and to the right
		// takes a parameter for the distance the particle should move
		void MoveSideWays(float distance);


		// a function to create the particle
		HRESULT CreateParticle();

		// a function to actually draw the model to the screen
		void DrawParticle(DirectX::XMMATRIX* view, DirectX::XMMATRIX* projection, DirectX::XMFLOAT3* cameraposition);

		// a function to draw one particle
		void DrawOne(Particle* one, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, DirectX::XMFLOAT3 cameraposition);

		//functions to set all of the positions of the particle
		void SetX(float X);
		void SetY(float Y);
		void SetZ(float Z);

		//functions to set all of the rotations of the particle
		void SetXAngle(float X);
		void SetYAngle(float Y);
		void SetZAngle(float Z);

		// a function to set the scale of the object
		void SetScale(float scale);

		//functions to get all of the positions of the particle
		float ReturnXPos();
		float ReturnYPos();
		float ReturnZPos();

		//functions of get all of the rotations of the particle
		float ReturnXAngle();
		float ReturnYAngle();
		float ReturnZAngle();

		// a function to get the scale of the particle
		float ReturnScale();

		// functions to change the particle positions
		void ChangeXPos(float ChangeAmount);
		void ChangeYPos(float ChangeAmount);
		void ChangeZPos(float ChangeAmount);

		//functions to change the particle rotations
		void ChangeXAngle(float ChangeAmount);
		void ChangeYAngle(float ChangeAmount);
		void ChangeZAngle(float ChangeAmount);

		//a function to change the particle scale
		void ChangeScale(float ChangeAmount);

		// a function to turn the particles on and off
		void SetEngine(bool OnOff);

};

