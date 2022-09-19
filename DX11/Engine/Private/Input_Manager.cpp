#include "Input_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CInput_Manager)

int g_Arr[(int)KEY::LAST] = {
	VK_LEFT,//LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	'1','2','3','4','5','6','7','8','9','0',
	VK_F1,VK_F2,VK_F3,VK_F4,
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	VK_MENU,//ALT,
	VK_LSHIFT,//LSHIFT,
	VK_SPACE,//SPACE,
	VK_CONTROL,//CTRL,
	VK_RETURN,//ENTER,
	VK_ESCAPE,//ESC,
	VK_TAB,
	VK_LBUTTON,
	VK_RBUTTON,
	VK_BACK,//BACKSPACE
	VK_DELETE,
	//LAST
};

KEY_STATE CInput_Manager::Get_KeyState(KEY _key)
{
	return m_vecKey[(int)_key].eState;
}

HRESULT CInput_Manager::Initialize(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	m_vecKey.reserve((UINT)KEY::LAST);

	for (int i = 0; i < (int)KEY::LAST; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });

	return S_OK;
}

HRESULT CInput_Manager::SetUp_DeviceState()
{
	if (nullptr == m_pMouse)
		return E_FAIL;

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return S_OK;
}

HRESULT CInput_Manager::Tick(_float fTimeDelta)
{
	/*if (GAMEINSTANCE->Get_Window_Handle() == GetFocus())
	{*/
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			if (GetAsyncKeyState(g_Arr[i]) & 0x8000) { // Ű�� ���� �ִ�
				if (m_vecKey[i].bPrevPush) // ������ ��������
					m_vecKey[i].eState = KEY_STATE::HOLD; // Ȧ���ΰɷ�
				else
					m_vecKey[i].eState = KEY_STATE::TAP; // �ƴϸ� �� ��

				m_vecKey[i].bPrevPush = true; // ���� ������ Ȱ��ȭ
			}
			else
			{ // Ű�� �ȴ����ִ�
				if (m_vecKey[i].bPrevPush) // ������ ���ȴٸ�
					m_vecKey[i].eState = KEY_STATE::AWAY;
				else
					m_vecKey[i].eState = KEY_STATE::NONE;

				m_vecKey[i].bPrevPush = false; // ���� ������ ��Ȱ��ȭ
			}
		}
	//}
	//else
	//{ // ��Ŀ���� null�̶��
	//	for (int i = 0; i < (int)KEY::LAST; ++i)
	//	{
	//		m_vecKey[i].bPrevPush = false;

	//		switch (m_vecKey[i].eState) {
	//		case KEY_STATE::TAP:
	//		case KEY_STATE::HOLD:
	//			m_vecKey[i].eState = KEY_STATE::AWAY;
	//			break;

	//		default:
	//			m_vecKey[i].eState = KEY_STATE::NONE;
	//			break;
	//		}
	//	}
	//}

	return S_OK;
}

void CInput_Manager::Free()
{
	__super::Free();

	m_pMouse->Release();

}
