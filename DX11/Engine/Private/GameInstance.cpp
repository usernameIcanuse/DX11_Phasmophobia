#include "..\Public\GameInstance.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pInput_Manager(CInput_Manager::Get_Instance())
	
{	

	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	/* 그래픽디바이스. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.isWindowMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	/* 인풋 디바이스. */
	if (FAILED(m_pInput_Manager->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;


	/* 오브젝트 매니져의 예약. */
	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약. */
	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)	
		return E_FAIL;

	m_pInput_Manager->SetUp_DeviceState();

	m_pLevel_Manager->Tick(fTimeDelta);	

	m_pObject_Manager->Tick(fTimeDelta);

	m_pInput_Manager->Tick(fTimeDelta);

	m_pObject_Manager->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();


	return S_OK;
}

HRESULT CGameInstance::Clear(_uint iLevelID)
{
	if (nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear(iLevelID);
	m_pComponent_Manager->Clear(iLevelID);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Present();

	return S_OK;
}

bool CGameInstance::Is_KeyState(KEY _Key, KEY_STATE _KeyState)
{
	if (nullptr == m_pInput_Manager)
		return false;

	return m_pInput_Manager->Get_KeyState(_Key) == _KeyState;
}

_byte CGameInstance::Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID)
{
	if (nullptr == m_pInput_Manager)
		return 0;

	return m_pInput_Manager->Get_DIMouseKeyState(eMouseButtonID);
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	if (nullptr == m_pInput_Manager)
		return 0;

	return m_pInput_Manager->Get_DIMouseMoveState(eMouseMove);
}

HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelID, pLevel);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, CGameObject** pOut ,void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pOut,pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr; 

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);	
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);	
}

_float CGameInstance::Compute_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_Timer(pTimerTag);
}




void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();		

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();	

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CInput_Manager::Get_Instance()->Destroy_Instance();
	
	CInput_Device::Get_Instance()->Destroy_Instance();

	

	CGraphic_Device::Get_Instance()->Destroy_Instance();

}

void CGameInstance::Free()
{

	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}
