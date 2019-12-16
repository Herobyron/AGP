#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>


class Window
{
private:

	HINSTANCE HInstance = NULL;
	HWND Hwnd = NULL;

	char TutorialName[100] = "PLS WORK";

public:

	Window();
	~Window();


	HRESULT InitialiseWindow(HINSTANCE hinstance, int CmdShow);

	HWND ReturnHWND();

};

