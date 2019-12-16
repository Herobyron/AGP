#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window* TheWindow = new Window();

	if (FAILED(TheWindow->InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("FAILED TO CREATE WINDOW");
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
		else
		{
			//do stuff
		}
	}

	return(int)msg.wParam;


}