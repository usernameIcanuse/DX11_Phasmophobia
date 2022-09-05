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

	if (FAILED(Setup_Component()))
		return E_FAIL;

	if (FAILED(Setup_Inventory()))
		return E_FAIL;
	if (FAILED(Setup_Camera()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Is_KeyState(KEY::W, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::S, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::A, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Is_KeyState(KEY::D, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_long		MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}



	if (pGameInstance->Is_KeyState(KEY::G, KEY_STATE::TAP))
	{
		m_pInventory->Drop_Item();
	}

	if (pGameInstance->Is_KeyState(KEY::Q, KEY_STATE::TAP))
	{
		m_pInventory->Change_Item();
	}

	if (pGameInstance->Is_KeyState(KEY::E, KEY_STATE::TAP))
	{
		if(m_pItem)
			m_pInventory->Add_Item(m_pItem);
	}

	if (pGameInstance->Is_KeyState(KEY::F, KEY_STATE::TAP))
	{
		if(m_bFlag)
			m_pInventory->Install_Item(m_vColliderPos);
	}

	m_pRayCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDist = FLT_MAX;
	m_bFlag = false;

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

HRESULT CPlayer::Setup_Component()
{
	/* For.Com_Ray*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::SIGHT;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_Ray"), TEXT("Com_Ray"), (CComponent**)&m_pRayCom, &ColliderDesc)))
		return E_FAIL;



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
	if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1, TEXT("Layer_Inventory"), TEXT("Prototype_GameObject_Inventory"), (CGameObject**)&m_pInventory, this)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::On_Collision_Enter(CCollider* pCollider)
{
	
}

void CPlayer::On_Collision_Stay(CCollider* pCollider)
{
	if (COLLISION_TYPE::ITEM == pCollider->Get_Type())
	{
		_float fCollisionDist = m_pRayCom->Get_Collision_Dist();

		if (DBL_EPSILON < fCollisionDist && m_fDist > fCollisionDist)
		{
			m_bFlag = true;
			m_fDist = fCollisionDist;
			m_pItem = pCollider->Get_Owner();
			m_vColliderPos = m_pRayCom->Get_CollidePos();
		}
	}
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

	Safe_Release(m_pRayCom);
	Safe_Release(m_pInventory);
}
