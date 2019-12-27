#pragma once
#include <dinput.h>


class Input
{
private:

    IDirectInput8* DirectInput;
	IDirectInputDevice8* KeyBoardDevice;

	unsigned char KeyBoardKeyState[256];

	HINSTANCE Instance;
	HWND Hwnd;


public:

	Input(HINSTANCE instance, HWND hwnd);
	~Input();

	HRESULT InitialiseInput(HINSTANCE instance, HWND hwnd);
	void ReadInputstates();
	bool ISKeyPressed(unsigned char DI_KeyCode);

};

