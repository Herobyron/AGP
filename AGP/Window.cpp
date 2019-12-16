#include "Window.h"

//callback
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Window::Window()
{

}

Window::~Window()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}


	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

}


HRESULT Window::InitialiseWindow(HINSTANCE hinstance, int CmdShow)
{
	//a name for the app
	char Name[100] = "PLEASE WORK";

	//register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hinstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	//create window
	HInstance = hinstance;
	RECT rc = { 0,0,640,480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	Hwnd = CreateWindow(Name, TutorialName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hinstance, NULL);

	if (!Hwnd)
		return E_FAIL;

	ShowWindow(Hwnd, CmdShow);

	return S_OK;

}