#pragma once
#include <d3d11.h>

class Window
{
private:

	HINSTANCE HInst;
	HWND HWnd;
	RECT rc;


public:

	Window(HINSTANCE hInstance, int CommandShow);
	~Window();

	HRESULT InitialiseWindow(HINSTANCE hInstance, int CommandShow);

	RECT GetRect();
	HWND GetHWND();
	HINSTANCE GetHInstance();

};

