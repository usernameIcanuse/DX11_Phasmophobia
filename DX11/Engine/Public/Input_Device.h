#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:

	
public:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte Get_DIKeyState(_ubyte byKeyID) {
		return m_KeyState[byKeyID];
	}	

	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID) {
		return m_MouseState.rgbButtons[eMouseButtonID];
	}

	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove) {
		return *((_long*)&m_MouseState + eMouseMove);
	}

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_DeviceState();

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	
private:
	_byte					m_KeyState[256];
	DIMOUSESTATE			m_MouseState;

public:
	virtual void Free() override;
};

END