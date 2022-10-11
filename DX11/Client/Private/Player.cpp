#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_FPS.h"
#include "Inventory.h"


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
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(70.f, 0.f, 52.f, 1.f));

	if (FAILED(Setup_Camera()))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_Inventory()))
		return E_FAIL;


	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Reset_Direction();

	if (pGameInstance->Is_KeyState(KEY::W, KEY_STATE::HOLD))
	{
		m_pTransformCom->Add_Direction(CTransform::FRONT);
	}

	if (pGameInstance->Is_KeyState(KEY::S, KEY_STATE::HOLD))
	{
		m_pTransformCom->Add_Direction(CTransform::BACK);
	}

	if (pGameInstance->Is_KeyState(KEY::A, KEY_STATE::HOLD))
	{
		m_pTransformCom->Add_Direction(CTransform::LEFT);
	}

	if (pGameInstance->Is_KeyState(KEY::D, KEY_STATE::HOLD))
	{
		m_pTransformCom->Add_Direction(CTransform::RIGHT);
	}

	if (pGameInstance->Is_KeyState(KEY::LEFT, KEY_STATE::HOLD))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f * fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::RIGHT, KEY_STATE::HOLD))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::UP, KEY_STATE::HOLD))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -1.f * fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::DOWN, KEY_STATE::HOLD))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta);
	}

	_long		MouseMove = 0;
	/*
	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}*/
	RELEASE_INSTANCE(CGameInstance);

	m_pTransformCom->Move(fTimeDelta/*,m_pNavigationCom*/);
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	
}

void CPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
#ifdef _DEBUG
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
#endif
	
}

HRESULT CPlayer::Render()
{
//#ifdef _DEBUG
//	m_pNavigationCom->Render();
//	//m_pOBBCom->Render();
//#endif
	return S_OK;
}

HRESULT CPlayer::Setup_Component()
{
	/* For.Com_OBB*/
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 12.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::PLAYER;
	ColliderDesc.fRayLength = 10.f;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
	

	/* For.Com_Navigation*/
	CGameObject* pNavigation = nullptr;

	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1,TEXT("Layer_Navigation"),TEXT("Prototype_GameObject_Navigation_Mesh"),&pNavigation)))
		return E_FAIL;

	m_pNavigationCom = (CNavigation*)pNavigation->Get_Component(TEXT("Com_Navigation"));
	Safe_AddRef(m_pNavigationCom);

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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_Camera_FPS"), &pCamera, &CameraDesc)))
		return E_FAIL;

	static_cast<CCamera_FPS*>(pCamera)->Set_Target(m_pTransformCom);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Setup_Inventory()
{
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Inventory"), TEXT("Prototype_GameObject_Inventory"), (CGameObject**)&m_pInventory, m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::On_Collision_Enter(CCollider* pCollider)
{
	if (COLLISION_TYPE::GHOST_ATTACK == pCollider->Get_Type())
	{
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Normal_Operation"));
		GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Normal_Operation"));
		/*Game_End()*/
	}
}

void CPlayer::On_Collision_Stay(CCollider* pCollider)
{ 
	if (COLLISION_TYPE::DOOR == pCollider->Get_Type())
	{

		CTransform* pDoorTransform = (CTransform*)pCollider->Get_Owner()->Get_Component(CGameObject::m_pTransformTag);
		_float3 vRight;
		XMStoreFloat3(&vRight, pDoorTransform->Get_State(CTransform::STATE_RIGHT));

		m_pTransformCom->Slide_Object(vRight, m_pNavigationCom);
		//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	}
	/*한 번 슬라이드 잘못 타면 붙어버림*/

}

void CPlayer::On_Collision_Exit(CCollider* pCollider)
{
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

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif

}
