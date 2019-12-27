#include "Input.h"

Input::Input(HINSTANCE instance, HWND hwnd)
{
	Instance = instance;
	Hwnd = hwnd;
}

Input::~Input()
{
	if (KeyBoardDevice) KeyBoardDevice->Unacquire();
	if (KeyBoardDevice) KeyBoardDevice->Release();

	if (DirectInput) DirectInput->Release();
}

HRESULT Input::InitialiseInput(HINSTANCE instance, HWND hwnd)
{
	HRESULT hr;
	ZeroMemory(KeyBoardKeyState, sizeof(KeyBoardKeyState));

	hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &KeyBoardDevice, NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = KeyBoardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = KeyBoardDevice->Acquire();

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}


void Input::ReadInputstates()
{
	HRESULT hr;

	hr = KeyBoardDevice->GetDeviceState(sizeof(KeyBoardKeyState), (LPVOID)&KeyBoardKeyState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			KeyBoardDevice->Acquire();
		}
	}
}

bool Input::ISKeyPressed(unsigned char DI_KeyCode)
{
	return KeyBoardKeyState[DI_KeyCode] & 0x80;
}
