#pragma once
#include "DirectXSetUp.h"
#include "Renderer.h"

class World
{
private:

	Renderer* TheRenderer;
	DirectXSetUp* TheSetUp;

public:

	World(HINSTANCE hInstance, int CommandShow);
	~World();



};

