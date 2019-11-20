#include "Window.h"

//callback 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE hInstance, int CommandShow)
{
	InitialiseWindow(hInstance, CommandShow);
}

Window::~Window()
{

}


HRESULT Window::InitialiseWindow(HINSTANCE hInstance, int CommandShow)
{
	
		// the name of the window
		char Name[100] = "Crashing VisualStudio Again (ROBO)";

		//register class
		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		//   wcex.hbrBackground = (HBRUSH )( COLOR_WINDOW + 1); // Needed for non-D3D apps
		wcex.lpszClassName = Name;

		if (!RegisterClassEx(&wcex)) return E_FAIL;

		// Create window
		HInst = hInstance;
		RECT rc = { 0, 0, 640, 480 };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		HWnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
			rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
		if (!HWnd)
			return E_FAIL;

		ShowWindow(HWnd, CommandShow);

		return S_OK;

	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_SIZE:
		{

			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);;
}


RECT Window::GetRect()
{
	return rc;
}

HWND Window::GetHWND()
{
	return HWnd;
}

HINSTANCE Window::GetHInstance()
{
	return HInst;
}
