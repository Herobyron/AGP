#include "Window.h"
#include "DirectSetUp.h"
#include "Render.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window* TheWindow = new Window();
	DirectSetUp* TheSetUp = new DirectSetUp();
	Render* TheRender = new Render();


	if (FAILED(TheWindow->InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("FAILED TO CREATE WINDOW");
		return 0;
	}

	if (FAILED(TheSetUp->InitialseD3D(TheWindow)))
	{
		DXTRACE_MSG("FAILED To CREATE DEVICE");
		return 0;
	}

	if (FAILED(TheRender->IntialiseGraphics(TheSetUp)))
	{
		DXTRACE_MSG("FAILED TO INTIALISE GRAPHICS");
		return 0;
	}

	//main loop
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		TheRender->RenderFrame(TheSetUp);
	}

	return(int)msg.wParam;


}