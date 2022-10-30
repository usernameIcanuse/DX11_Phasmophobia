#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_FPS.h"
#include "Inventory.h"
#include "Journal.h"
#include "Level_Loading.h"
#include "SoundMgr.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4((_float4*)pArg));

	}

	CSoundMgr::Get_Instance()->Set_Player(this);
	
	if (FAILED(Setup_Camera()))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_Inventory()))
		return E_FAIL;
	if (FAILED(Setup_Journal()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Current_Camera(TEXT("Camera_Player"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	m_bLockCursor = pGameInstance->Get_Mouse_Lock();

	if (pGameInstance->Is_KeyState(KEY::ESC, KEY_STATE::TAP))
	{
		pGameInstance->Set_Mouse_Lock();
		m_bLockCursor =pGameInstance->Get_Mouse_Lock();;
		m_pJournal->Set_Enable(!m_bLockCursor);
		m_pJournal->Main_On(!m_bLockCursor);
		if (true == m_bLockCursor)
			m_pJournal->Off_AllUI();
	}

	if (pGameInstance->Is_KeyState(KEY::J, KEY_STATE::TAP))
	{
		pGameInstance->Set_Mouse_Lock();
		m_bLockCursor = pGameInstance->Get_Mouse_Lock();;
		m_pJournal->Set_Enable(!m_bLockCursor);
		m_pJournal->Evidence_On(!m_bLockCursor);
		if (true == m_bLockCursor)
			m_pJournal->Off_AllUI();
	}

	if (false == m_bLockCursor)//UI켰을 때
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	m_fFootstepTime -= fTimeDelta;
	m_pTransformCom->Reset_Direction();

	if (pGameInstance->Is_KeyState(KEY::W, KEY_STATE::HOLD))
	{
		if (0.f > m_fFootstepTime)
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Player_FootStep.wav"), CSoundMgr::CHANNEL_PLAYER,0.6f);
			m_fFootstepTime = 1.f;
		}
		m_pTransformCom->Add_Direction(CTransform::FRONT);
	}

	if (pGameInstance->Is_KeyState(KEY::S, KEY_STATE::HOLD))
	{
		if (0.f > m_fFootstepTime)
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Player_FootStep.wav"), CSoundMgr::CHANNEL_PLAYER, 0.6f);
			m_fFootstepTime = 1.f;
		}
		m_pTransformCom->Add_Direction(CTransform::BACK);
	}

	if (pGameInstance->Is_KeyState(KEY::A, KEY_STATE::HOLD))
	{
		if (0.f > m_fFootstepTime)
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Player_FootStep.wav"), CSoundMgr::CHANNEL_PLAYER, 0.6f);
			m_fFootstepTime = 1.f;
		}
		m_pTransformCom->Add_Direction(CTransform::LEFT);
	}

	if (pGameInstance->Is_KeyState(KEY::D, KEY_STATE::HOLD))
	{
		if (0.f > m_fFootstepTime)
		{
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Player_FootStep.wav"), CSoundMgr::CHANNEL_PLAYER, 0.6f);
			m_fFootstepTime = 1.f;
		}
		m_pTransformCom->Add_Direction(CTransform::RIGHT);
	}

	//if (pGameInstance->Is_KeyState(KEY::LEFT, KEY_STATE::HOLD))
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f * fTimeDelta);
	//}

	//if (pGameInstance->Is_KeyState(KEY::RIGHT, KEY_STATE::HOLD))
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	//}

	//if (pGameInstance->Is_KeyState(KEY::UP, KEY_STATE::HOLD))
	//{
	//	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -1.f * fTimeDelta);
	//}

	//if (pGameInstance->Is_KeyState(KEY::DOWN, KEY_STATE::HOLD))
	//{
	//	m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta);
	//}

	_long		MouseMove = 0;
	
	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	m_pTransformCom->Move(fTimeDelta,m_pCurrNavigation);
	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	
	if (true == m_bIsInHouse)
	{
		pGameInstance->Add_Desc(CEvent_Manager::HOUSETIME, fTimeDelta);
	}

	if (true == m_bIsInGhostArea)
	{
		pGameInstance->Add_Desc(CEvent_Manager::AROUNDGHOST, fTimeDelta);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
#ifdef _DEBUG
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	m_pRendererCom->Add_DebugRenderGroup(m_pCurrNavigation);
#endif
	
}

HRESULT CPlayer::Render()
{

	return S_OK;
}

_bool CPlayer::Picking_Navigation(RAY tMouseRay, _float4& vPickedPos)
{
	if (nullptr == m_pCurrNavigation)
		return false;

	if (m_pCurrNavigation == m_pNaviHouseCom)
	{
		return m_pCurrNavigation->Picking_Mesh(tMouseRay, vPickedPos);
	}
	return false;
}

_int CPlayer::Get_CurrentIndex()
{
	if (nullptr != m_pCurrNavigation)
	{
		return m_pCurrNavigation->Get_CurrentIndex();
	}
	return -1;
}

HRESULT CPlayer::Setup_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	/* For.Com_AABB*/
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 12.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::PLAYER;
	ColliderDesc.fRayLength = 10.f;

	if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	


	/* For.Com_Navigation*/

	CNavigation::NAVIDESC	NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	NaviDesc.m_iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_House"), TEXT("Com_NaviHouse"), (CComponent**)&m_pNaviHouseCom, &NaviDesc)))
		return E_FAIL;

	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	NaviDesc.m_iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(pGameInstance->Get_Next_Level(), TEXT("Prototype_Component_Navigation_OutSide"), TEXT("Com_NaviOutSide"), (CComponent**)&m_pNaviOutSideCom, &NaviDesc)))
		return E_FAIL;

	m_pCurrNavigation = m_pNaviOutSideCom;

	RELEASE_INSTANCE(CGameInstance);


#ifdef _DEBUG
	/*For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CPlayer::Setup_Camera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	_float4		vPos,vAt;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	XMStoreFloat4(&vAt, m_pTransformCom->Get_State(CTransform::STATE_LOOK) + m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	CameraDesc.vEye = vPos;
	CameraDesc.vAt = vAt;
	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CGameObject* pCamera = nullptr;

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera_FPS"), &pCamera, &CameraDesc)))
		return E_FAIL;

	static_cast<CCamera_FPS*>(pCamera)->Set_Target(m_pTransformCom);

	if (FAILED(pGameInstance->Add_CameraObject(TEXT("Camera_Player"), (CCamera*)pCamera)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Setup_Inventory()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Inventory"), TEXT("Prototype_GameObject_Inventory"), (CGameObject**)&m_pInventory, this)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance)
	return S_OK;
}

HRESULT CPlayer::Setup_Journal()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UI"), TEXT("Prototype_GameObject_Journal"), (CGameObject**)&m_pJournal)))
		return E_FAIL;

	m_pJournal->Set_Enable(false);

	RELEASE_INSTANCE(CGameInstance)

	return S_OK;
}

void CPlayer::On_Collision_Enter(CCollider* pCollider)
{
	if (COLLISION_TYPE::GHOST_ATTACK == pCollider->Get_Type())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Normal_Operation"));
		pGameInstance->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Normal_Operation"));
		
		pGameInstance->Add_ReserveLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY,false,true), LEVEL_LOBBY);

		RELEASE_INSTANCE(CGameInstance);
	}

	else if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
	{
		m_pCurrNavigation = m_pNaviHouseCom;
		m_bIsInHouse = true;
	}

	else if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
	{
		m_bIsInGhostArea = true;
	}

}

void CPlayer::On_Collision_Stay(CCollider* pCollider)
{ 
	if (COLLISION_TYPE::DOOR == pCollider->Get_Type())
	{

		CTransform* pDoorTransform = (CTransform*)pCollider->Get_Owner()->Get_Component(CGameObject::m_pTransformTag);
		_float3 vRight;
		XMStoreFloat3(&vRight, pDoorTransform->Get_State(CTransform::STATE_RIGHT));

		m_pTransformCom->Slide_Object(vRight, m_pCurrNavigation);
		
	}
	/*한 번 슬라이드 잘못 타면 붙어버림*/

}

void CPlayer::On_Collision_Exit(CCollider* pCollider)
{
	if (COLLISION_TYPE::HOUSE == pCollider->Get_Type())
	{
		m_pCurrNavigation = m_pNaviOutSideCom;
		m_bIsInHouse = false;
	}
	else if (COLLISION_TYPE::GHOST_AREA == pCollider->Get_Type())
	{
		m_bIsInGhostArea = false;
	}
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pNaviHouseCom);
	Safe_Release(m_pNaviOutSideCom);
	Safe_Release(m_pAABBCom);

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif

}
