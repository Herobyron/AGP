#pragma once
#include <dinput.h>


class Input
{
private:

	// a pointer to the direct input
    IDirectInput8* DirectInput;
	
	// a pointer to the keyboard device
	IDirectInputDevice8* KeyBoardDevice;

	// a char which holds what key is currently bieng used
	unsigned char KeyBoardKeyState[256];

	//the instance to be used for input setup
	HINSTANCE Instance;
	
	// the HWND 
	HWND Hwnd;


public:

	// the inputs constructor which takes an instance and the HWND from the windows class
	Input(HINSTANCE instance, HWND hwnd);

	// inputs destructor
	~Input();

	// a function to initialise the input which takes in a parameters of an instance and HWND
	HRESULT InitialiseInput(HINSTANCE instance, HWND hwnd);
	
	// a function that takes in the input state that is currently bieng used
	void ReadInputstates();
	
	//checks to see what key is pressed 
	bool ISKeyPressed(unsigned char DI_KeyCode);

};

