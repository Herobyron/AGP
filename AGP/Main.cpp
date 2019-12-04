#include <d3d11.h>
#include "Renderer.h"
#include "Window.h"
#include "DirectXSetUp.h"
#include "Camera.h"

int WINAPI WinMain(_In_ HINSTANCE hinstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Renderer* renderer = new Renderer(hinstance, nCmdShow);
	Camera* WorldCamera = new Camera(0.0f, 0.0f, -1, 0.0f);




	renderer->ReturnDirectX()->InitialiseD3D();
	renderer->InitialseGraphics();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer->RenderUpdate(WorldCamera);
		}




	}






}


