#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>


class Window
{
private:

	// a vairable for the HInstance
	HINSTANCE HInstance = NULL;

	// a variable for the HWND
	HWND Hwnd = NULL;

	// the name of the window
	char TutorialName[100] = "Byrons AGP";

public:

	// the window constructor
	Window();

	// the window destructor
	~Window();

	// Initialise window function that uses the hinstance and cmnd show to create the window
	HRESULT InitialiseWindow(HINSTANCE hinstance, int CmdShow);

	// returns a refernce to the HWND
	HWND ReturnHWND();
	
	// returns a refernce to the HInstance
	HINSTANCE ReturnHinstance();
};

