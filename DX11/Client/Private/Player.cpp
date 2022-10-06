#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_FPS.h"
#include "Inventory.h"
#include "Door.h"

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


	if (pGameInstance->Is_KeyState(KEY::W, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
	}

	if (pGameInstance->Is_KeyState(KEY::S, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
	}

	if (pGameInstance->Is_KeyState(KEY::A, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Left(fTimeDelta, m_pNavigationCom);
	}

	if (pGameInstance->Is_KeyState(KEY::D, KEY_STATE::HOLD))
	{
		m_pTransformCom->Go_Right(fTimeDelta, m_pNavigationCom);
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

	if (pGameInstance->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
	{
		if (nullptr != m_pDoor)
		{
			m_bGrab = true;
			m_pDoor->Grab_Door(this);
		}
	}
	else if (pGameInstance->Is_KeyState(KEY::LBUTTON, KEY_STATE::AWAY))
	{
		if (nullptr != m_pDoor)
		{
			m_bGrab = false;
			m_pDoor->Grab_Door(nullptr);
			m_pDoor = nullptr;
		}

	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
	{
		m_pInventory->Frequency_Control(MouseMove/100);
	}


	if (pGameInstance->Is_KeyState(KEY::RBUTTON, KEY_STATE::TAP))
	{
		m_pInventory->Turn_Switch();
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
			m_pInventory->Add_Item((CItem*)m_pItem);
	}

	
	if (m_eColliderType != COLLISION_TYPE::TYPE_END )
	{
		if (m_eColliderType == COLLISION_TYPE::TRIPOD)
			m_pInventory->Item_TempModel(m_vColliderPos, m_eColliderType, m_vColliderLook, (CItem*)m_pTripod);
		else
			m_pInventory->Item_TempModel(m_vColliderPos, m_eColliderType, m_vColliderLook);

		if (pGameInstance->Is_KeyState(KEY::F, KEY_STATE::TAP))
		{
			if(m_eColliderType == COLLISION_TYPE::TRIPOD)
				m_pInventory->Install_Item(m_vColliderPos, m_eColliderType, m_vColliderLook,(CItem*)m_pTripod);
			else
				m_pInventory->Install_Item(m_vColliderPos, m_eColliderType, m_vColliderLook);
		}
	}

	m_pRayCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDist = FLT_MAX;
	m_eColliderType = COLLISION_TYPE::TYPE_END;
	m_vColliderLook = _float4(0.f, 1.f, 0.f, 0.f);
	m_pTripod = nullptr;
	m_pItem = nullptr;
	if(!m_bGrab)
		m_pDoor = nullptr;

	RELEASE_INSTANCE(CGameInstance);
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
	/* For.Com_Ray*/
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::SIGHT;
	ColliderDesc.fRayLength = 10.f;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_Ray"), TEXT("Com_Ray"), (CComponent**)&m_pRayCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB*/
	
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(5.f, 12.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::PLAYER;
	ColliderDesc.fRayLength = 10.f;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;
	

	/* For.Com_Navigation*/
	//CGameObject* pNavigation = nullptr;

	//if (FAILED(GAMEINSTANCE->Add_GameObject(LEVEL_STAGE1,TEXT("Layer_Navigation"),TEXT("Prototype_GameObject_Navigation_Mesh"),&pNavigation)))
	//	return E_FAIL;

	//m_pNavigationCom = (CNavigation*)pNavigation->Get_Component(TEXT("Com_Navigation"));
	//Safe_AddRef(m_pNavigationCom);

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
	
}

void CPlayer::On_Collision_Stay(CCollider* pCollider)
{
	if (COLLISION_TYPE::ITEM == pCollider->Get_Type() || COLLISION_TYPE::THERMOMETER == pCollider->Get_Type() ||
		COLLISION_TYPE::CAMERA== pCollider->Get_Type()|| COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type()||
		COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		_float fCollisionDist = m_pRayCom->Get_Collision_Dist();

		if (DBL_EPSILON < fCollisionDist && m_fDist > fCollisionDist)
		{
			m_fDist = fCollisionDist;
			m_pItem = pCollider->Get_Owner();	
		}
	}
	else if (COLLISION_TYPE::OBJECT == pCollider->Get_Type() || COLLISION_TYPE::WALL== pCollider->Get_Type() ||
			COLLISION_TYPE::TRIPOD == pCollider->Get_Type()|| COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		_float fCollisionDist = m_pRayCom->Get_Collision_Dist();

		if (DBL_EPSILON < fCollisionDist && m_fDist > fCollisionDist)
		{
			m_fDist = fCollisionDist;
			m_vColliderPos = m_pRayCom->Get_CollidePos();
			m_eColliderType = pCollider->Get_Type();
			if(COLLISION_TYPE::WALL == pCollider->Get_Type())
				XMStoreFloat4(&m_vColliderLook ,static_cast<CTransform*>(pCollider->Get_Owner()->Get_Component(CGameObject::m_pTransformTag))->Get_State(CTransform::STATE_LOOK));
			else if (COLLISION_TYPE::TRIPOD == pCollider->Get_Type())
			{
				BoundingBox* pBoundingBox = nullptr;
				pBoundingBox = (BoundingBox*)pCollider->Get_Collider();
				m_vColliderPos = pBoundingBox->Center;
				m_pTripod = pCollider->Get_Owner();
				XMStoreFloat4(&m_vColliderLook, static_cast<CTransform*>(pCollider->Get_Owner()->Get_Component(CGameObject::m_pTransformTag))->Get_State(CTransform::STATE_UP));
			}
		}
	}
	else if (COLLISION_TYPE::DOOR == pCollider->Get_Type())
	{
		m_pDoor = (CDoor*)pCollider->Get_Owner();
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
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pOBBCom);

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif

}
