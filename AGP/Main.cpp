#include "Window.h"
#include "DirectSetUp.h"
#include "Render.h"
#include "Input.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window* TheWindow = new Window();
	DirectSetUp* TheSetUp = new DirectSetUp();
	Render* TheRender = new Render();
	Input* TheInput = new Input(TheWindow->ReturnHinstance(), TheWindow->ReturnHWND());


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

	//if (FAILED(TheInput->InitialiseInput(TheWindow->ReturnHinstance(), TheWindow->ReturnHWND())))
	//{
	//	DXTRACE_MSG("FAILED INPUT INITIALISATION");
	//	return 0;
	//}

	TheInput->InitialiseInput(TheWindow->ReturnHinstance(), TheWindow->ReturnHWND());

	//main loop
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		TheInput->ReadInputstates();
		if (TheInput->ISKeyPressed(DIK_W))
		{
			TheRender->ReturnCamera()->Foward(1.0f * 0.01);
		}
		else if (TheInput->ISKeyPressed(DIK_S))
		{
			TheRender->ReturnCamera()->Foward(-1.0f * 0.01);
		}
		else if (TheInput->ISKeyPressed(DIK_Q))
		{
			TheRender->ReturnCamera()->Up(1.0f * 0.01);
		}
		else if (TheInput->ISKeyPressed(DIK_E))
		{
			TheRender->ReturnCamera()->Up(-1.0f * 0.01);
		}
		else if (TheInput->ISKeyPressed(DIK_A))
		{
			TheRender->ReturnCamera()->Rotate(-1.0f * 0.1);
		}
		else if (TheInput->ISKeyPressed(DIK_D))
		{
			TheRender->ReturnCamera()->Rotate(1.0f * 0.1);
		}

	
		TheRender->RenderFrame(TheSetUp, TheInput);
		
	}

	return(int)msg.wParam;


}