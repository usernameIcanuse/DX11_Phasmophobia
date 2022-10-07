#include "..\Public\GameInstance.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pInput_Manager(CInput_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pGame_Manager(CGame_Manager::Get_Instance())
	, m_pRenderer_Manager(CRenderer_Manager::Get_Instance())
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
{	

	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pCollision_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pGame_Manager);
	Safe_AddRef(m_pRenderer_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	m_tagGraphicDesc = GraphicDesc;
	/* 그래픽디바이스. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.isWindowMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	/* 인풋 디바이스. */
	if (FAILED(m_pInput_Manager->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	m_pCollision_Manager->Initialize();
		
	m_pFrustum->Initialize();

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

	m_pInput_Manager->Tick(fTimeDelta);

	m_pFrustum->Clear_Frustum();

	m_pObject_Manager->Tick(fTimeDelta);

	m_pGame_Manager->Tick(fTimeDelta);

	m_pCollision_Manager->Tick();

	m_pPipeLine->Tick();

	m_pFrustum->Update();

	m_pObject_Manager->LateTick(fTimeDelta);

	m_pFrustum->Late_Update();

	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	
	if (nullptr == m_pRenderer_Manager)
		return E_FAIL;

	m_pRenderer_Manager->Draw_RenderGroup();

	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();

	m_pLight_Manager->Clear_Light();


	return S_OK;
}

HRESULT CGameInstance::Add_Renderer(_uint eListIndex, CRenderer* pRenderer)
{
	if (nullptr == m_pRenderer_Manager)
		return E_FAIL;

	return m_pRenderer_Manager->Add_Renderer(eListIndex,pRenderer);
}

HRESULT CGameInstance::Draw_RenderGroup()
{
	if (nullptr == m_pRenderer_Manager)
		return E_FAIL;

	return m_pRenderer_Manager->Draw_RenderGroup();
}

void CGameInstance::Clear_RendererIndex(_uint eListIndex)
{
	if (nullptr == m_pRenderer_Manager)
		return;

	m_pRenderer_Manager->Clear_RendererIndex(eListIndex);
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

GRAPHICDESC CGameInstance::Get_GraphicDesc()
{
	return m_tagGraphicDesc;
}

ID3D11Device* CGameInstance::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;
	return m_pGraphic_Device->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_Context()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Context();
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

_uint CGameInstance::Get_Current_Level()
{
	if (nullptr == m_pLevel_Manager)
		return -1;
	return m_pLevel_Manager->Get_Current_Level();
}

void CGameInstance::Set_Current_Level(_uint _iCurrentLevelID)
{
	if (nullptr == m_pLevel_Manager)
		return;
	return m_pLevel_Manager->Set_Current_Level(_iCurrentLevelID);

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

void CGameInstance::Clear_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return;

	return m_pObject_Manager->Clear_Layer(iLevelIndex, pLayerTag);
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

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformState);
}

_matrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eState);
}

const _float4x4* CGameInstance::Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_float4x4(eState);
}

 _float4x4* CGameInstance::Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_TP(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

HRESULT CGameInstance::Add_Light(CLight* pLight)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pLight);
}

LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

void CGameInstance::Clear_Light()
{
	if (nullptr == m_pLight_Manager)
		return;

	m_pLight_Manager->Clear_Light();
}

void CGameInstance::Add_ItemFrustum(_uint eFrustumType, CRenderer* _pRenderer, CTransform* _pTransform)
{
	if (nullptr == m_pFrustum)
		return;

	m_pFrustum->Add_ItemFrustum((CFrustum::FRUSTUM_TYPE)eFrustumType, _pRenderer, _pTransform);
}

void CGameInstance::Add_Object_For_Culling(CGameObject* pGameObject, CRenderer::RENDERGROUP eRenderGroup)
{
	if (nullptr == m_pFrustum)
		return;

	m_pFrustum->Add_Object_For_Culling(pGameObject, eRenderGroup);
}


_bool CGameInstance::isIn_Frustum_InLocalSpace(_fvector vLocalPoint, _float fRange, CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_Frustum_InLocalSpace(vLocalPoint, fRange, eRenderGroup, pGameObject);

}

void CGameInstance::Transform_ToLocalSpace(_fmatrix WorldMatrixInv)
{
	if (nullptr == m_pFrustum)
		return ;

	m_pFrustum->Transform_ToLocalSpace(WorldMatrixInv);

}

void CGameInstance::Add_Collider(CCollider* pCollider)
{
	m_pCollision_Manager->Add_Collider(pCollider);
}

void CGameInstance::Erase_Collider(CCollider* pCollider)
{
	m_pCollision_Manager->Erase_Collider(pCollider);
}

HRESULT CGameInstance::Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	if(nullptr == m_pFont_Manager)
		return E_FAIL;

	
	m_pFont_Manager->Add_Font(pDevice,  pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;


	m_pFont_Manager->Render_Font(pFontTag, pString, vPosition, vColor);
}

HRESULT CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;


	m_pFont_Manager->Render_Font(pFontTag, pString, vPosition, vColor, rotation, vOrigin, vScale);
}

HRESULT CGameInstance::Add_EventObject(_int iIndex, CGameObject* pObject)
{
	if (nullptr == m_pGame_Manager)
		return E_FAIL;
	return m_pGame_Manager->Add_EventObject(iIndex,pObject);
}

void CGameInstance::Broadcast_Message(_int iIndex, const _tchar* pMessage)
{
	if (nullptr == m_pGame_Manager)
		return;

	m_pGame_Manager->Broadcast_Message(iIndex, pMessage);
}

void CGameInstance::Clear_Layer(_int iIndex)
{
	if (nullptr == m_pGame_Manager)
		return;

	m_pGame_Manager->Clear_Layer(iIndex);
}

void CGameInstance::Clear_List()
{
	if (nullptr == m_pGame_Manager)
		return;

	m_pGame_Manager->Clear_List();
}



void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();		

	CRenderer_Manager::Get_Instance()->Destroy_Instance();

	CTarget_Manager::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();	

	CLight_Manager::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CInput_Manager::Get_Instance()->Destroy_Instance();

	CPipeLine::Get_Instance()->Destroy_Instance();

	CFrustum::Get_Instance()->Destroy_Instance();

	CCollision_Manager::Get_Instance()->Destroy_Instance();

	CFont_Manager::Get_Instance()->Destroy_Instance();

	CGame_Manager::Get_Instance()->Destroy_Instance();
	
	CGraphic_Device::Get_Instance()->Destroy_Instance();

	

}

void CGameInstance::Free()
{

	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pRenderer_Manager);
	Safe_Release(m_pGame_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pPipeLine);



}
