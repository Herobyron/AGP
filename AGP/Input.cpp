#include "Input.h"

Input::Input()
{
	DirectInput = NULL;
	KeyBoardDevice = NULL;
	Instance = NULL;
	Hwnd = NULL;
}

Input::Input(HINSTANCE INSTANCE, HWND hwnd)
	:Instance{ INSTANCE }, Hwnd{hwnd}
{
	IntialiseInput(Instance, Hwnd);
}

Input::~Input()
{
	if (KeyBoardDevice)
	{
		KeyBoardDevice->Unacquire();
		KeyBoardDevice->Release();
	}

	if (DirectInput) DirectInput->Release();
}


HRESULT Input::IntialiseInput(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT hr;
	ZeroMemory(KeyBoardKeysState, sizeof(KeyBoardKeysState));

	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& DirectInput, NULL);
	if (FAILED(hr)) return hr;

	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &KeyBoardDevice, NULL);
	if (FAILED(hr)) return hr;

	hr = KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	hr = KeyBoardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	hr = KeyBoardDevice->Acquire();
	if (FAILED(hr)) return hr;


	return S_OK;
}

void Input::ReadInputStates()
{
	HRESULT hr;
	hr = KeyBoardDevice->GetDeviceState(sizeof(KeyBoardKeysState), (LPVOID)& KeyBoardKeysState);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			KeyBoardDevice->Acquire();
		}
	}
}

bool Input::IsKeyPressed(unsigned char DI_keycode)
{
	return KeyBoardKeysState[DI_keycode] & 0x80;
}