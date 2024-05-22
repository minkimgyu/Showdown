////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	bool keySixDown = false;
	bool keySevenDown = false;
	bool keyEightDown = false;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputClass::ProcessInput(GraphicsClass& graphics, HWND hwnd)
{
	CameraClass* camera = graphics.GetCameraClass();

	if (m_keyboardState[DIK_A] & 0x80)
	{
		camera->MoveHorizontal(true);
	}
	if (m_keyboardState[DIK_D] & 0x80)
	{
		camera->MoveHorizontal(false);
	}
	if (m_keyboardState[DIK_W] & 0x80)
	{
		camera->MoveVertical(true);
	}
	if (m_keyboardState[DIK_S] & 0x80)
	{
		camera->MoveVertical(false);
	}
	if (m_keyboardState[DIK_G] & 0x80)
	{
		if (graphics.GetStart() == false)
		{
			graphics.SetStart(true);
		}
	}
	//if (m_keyboardState[DIK_Q] & 0x80)
	//{
	//	XMFLOAT3 matrix = camera->GetPosition();
	//	camera->SetPosition(matrix.x, matrix.y + 0.01f, matrix.z);	// for chair
	//}
	//if (m_keyboardState[DIK_E] & 0x80)
	//{
	//	XMFLOAT3 matrix = camera->GetPosition();
	//	camera->SetPosition(matrix.x, matrix.y - 0.01f, matrix.z);	// for chair
	//}

	///////////////**************new**************////////////////////

	if (graphics.GetStart() == true)
	{
		if (m_mouseState.rgbButtons[0])
		{
			graphics.OnMouseDown();
		}
		if (!m_mouseState.rgbButtons[0])
		{
			graphics.OnMouseUp();
		}
	}
	
	///////////////**************new**************////////////////////

	if ((m_mouseState.lX != m_mouseLastState.lX) || (m_mouseState.lY != m_mouseLastState.lY))
	{
		camera->MoveMouse(m_mouseState, m_mouseLastState);
		m_mouseLastState = m_mouseState;
	}

	return;
}

void InputClass::ChangeTime(GraphicsClass& graphics)
{
	if (m_keyboardState[DIK_6] & 0x80)
	{
		graphics.ChangeTime(Day::Afternoon);
	}
	else if (m_keyboardState[DIK_7] & 0x80)
	{
		graphics.ChangeTime(Day::SunSet);
	}
	else if (m_keyboardState[DIK_8] & 0x80)
	{
		graphics.ChangeTime(Day::Night);
	}
}


void InputClass::SetLightColor(LightClass& light)
{
	
}

void InputClass::Start(GraphicsClass& graphics)
{
}

void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	return true;
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd)
{

	HRESULT result;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse); //»õ·Î Ãß°¡µÊ

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}