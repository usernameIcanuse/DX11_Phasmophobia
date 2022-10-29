#pragma once
#include "Base.h"

BEGIN(Engine)

class CInput_Manager final :
    public CBase
{
    DECLARE_SINGLETON(CInput_Manager)

	struct tKeyInfo {
		KEY_STATE eState;
		bool bPrevPush; // 이전 프레임 체크 용도
	};

private:
	CInput_Manager() = default;
	virtual ~CInput_Manager() = default;

public:

	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID) {
		return m_MouseState.rgbButtons[(_uint)eMouseButtonID];
	}

	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove) {
		return *((_long*)&m_MouseState + (_uint)eMouseMove);
	}

	const vector<tKeyInfo>& Get_vecKey() { return m_vecKey; }

	KEY_STATE				Get_KeyState(KEY _key);

	void					Set_Mouse_Lock()
	{
		m_bLockCursor = !m_bLockCursor;
	}

	_bool				Get_Mouse_Lock()
	{
		return m_bLockCursor;
	}
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_DeviceState();

	HRESULT Tick(_float fTimeDelta);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	
private:
	DIMOUSESTATE			m_MouseState;

	vector<tKeyInfo>	m_vecKey; // 벡터 안의 인덱스가 곧 키 값을 의미함
	_bool				m_bLockCursor = true;

public:
	virtual void Free() override;
};

END