#pragma once
#include <dinput.h>


class Input
{
private:

	// a pointer to the DirectInput Interface
	IDirectInput8* DirectInput;
	
	// a pointer to the Keyboard Interface
	IDirectInputDevice8* KeyBoardDevice;
	
	//which key state has been pressed
	unsigned char KeyBoardKeysState[256];

	//A refernce to the HInstance from main
	HINSTANCE Instance;

	// a refernce to the HWND from main
	HWND Hwnd;

public:

	Input();
	Input(HINSTANCE hinstance, HWND hwnd);
	~Input();

	HRESULT IntialiseInput(HINSTANCE hinstance, HWND hwnd);
	void ReadInputStates();
	bool IsKeyPressed(unsigned char DI_keycode);

};

