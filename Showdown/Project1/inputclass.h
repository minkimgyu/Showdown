////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>
#include "cameraclass.h"
#include "lightclass.h"
#include "graphicsclass.h"
#include "pickclass.h"
#include "bottleclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

	void ProcessInput(GraphicsClass&, HWND);
	void ChangeTime(GraphicsClass&);
	void SetLightColor(LightClass&);
	void Start(GraphicsClass&);

private:
	bool ReadKeyboard();
	bool ReadMouse();


private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_mouseLastState;

	bool keySixDown;
	bool keySevenDown;
	bool keyEightDown;
	bool rideCarOnce = false;

	//int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif