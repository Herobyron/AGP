#include <d3d11.h>
#include "Renderer.h"
#include "Window.h"
#include "DirectxSetup.h"

int WINAPI WinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//creates and intialsieses window
	Window* MainWindow = new Window(hinstance, nCmdShow);

	//creates direct set up and intialsies D3D
	DirectXSetUp* SetUp = new DirectXSetUp(MainWindow);

	//creates the Input and initialises inputs
	Input* TheInputs = new Input(MainWindow->GetHInstance(), MainWindow->GetHWND());

	//creates the Renderer and then Intialises Graphics
	Renderer* renderer = new Renderer(SetUp, TheInputs);

	//need to create input and other stuff as well before camera

	//creating the world camera
	Camera* WorldCamera = new Camera(0.0f, 0.0f, -10, 0.0f);



	//SetUp->InitialiseD3D();
	//renderer->InitialseGraphics();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

			renderer->RenderUpdate(WorldCamera);
		




	}






}


