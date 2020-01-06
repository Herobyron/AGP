#include "Input.h"
// the input base constructor
Input::Input(HINSTANCE instance, HWND hwnd)
{
	Instance = instance;
	Hwnd = hwnd;
}
// inputs basic destructor
Input::~Input()
{
	if (KeyBoardDevice) KeyBoardDevice->Unacquire();
	if (KeyBoardDevice) KeyBoardDevice->Release();

	if (DirectInput) DirectInput->Release();
}

// a function that intialises the inputs for the game
HRESULT Input::InitialiseInput(HINSTANCE instance, HWND hwnd)
{
	HRESULT hr;
	ZeroMemory(KeyBoardKeyState, sizeof(KeyBoardKeyState));

	//createing the input
	hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	//creating the input device
	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &KeyBoardDevice, NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	// sets the format for the keyboard
	hr = KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr))
	{
		return hr;
	}

	//setting the keyboard device
	hr = KeyBoardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return hr;
	}

	//aquires the keyboard
	hr = KeyBoardDevice->Acquire();

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

// a function to read the input states
void Input::ReadInputstates()
{
	HRESULT hr;

	// get the current state of the linked keyboard
	hr = KeyBoardDevice->GetDeviceState(sizeof(KeyBoardKeyState), (LPVOID)&KeyBoardKeyState);

	// if the keyboard hasnt been aquired then aquire it 
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			KeyBoardDevice->Acquire();
		}
	}
}

// a function that returns the bool depnding on if a key has been pressed
bool Input::ISKeyPressed(unsigned char DI_KeyCode)
{
	return KeyBoardKeyState[DI_KeyCode] & 0x80;
}
