#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Video_Camera.h"
#include "Door.h"


CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CInventory::CInventory(const CInventory& rhs)
	:CGameObject(rhs)
{
}

HRESULT CInventory::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;


	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{

	if (pArg)
	{
		m_pPlayerTransform = (CTransform*)pArg;
	}

	m_vInventory.reserve(3);

	for (int i = 0; i < 3; ++i)
	{
		CItem* pNull = nullptr;
		m_vInventory.push_back(pNull);
	}
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;


	if (FAILED(Setup_Component()))
		return E_FAIL;
	/* ax + by + cz + d =0*/
	//(a,b,c,d) = XMPlaneFromPoints(p1,p2,p3)

	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_vInventory[m_iIndex])
	{
		m_vInventory[m_iIndex]->Adjust_Item(m_pPlayerTransform);
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


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

	_long		MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(MMS_WHEEL))
	{
		Frequency_Control(MouseMove / 100);
	}


	if (pGameInstance->Is_KeyState(KEY::RBUTTON, KEY_STATE::TAP))
	{
		Turn_Switch();
	}

	if (pGameInstance->Is_KeyState(KEY::G, KEY_STATE::TAP))
	{
		Drop_Item();
	}

	if (pGameInstance->Is_KeyState(KEY::Q, KEY_STATE::TAP))
	{
		Change_Item();
	}

	if (pGameInstance->Is_KeyState(KEY::E, KEY_STATE::TAP))
	{
		if (m_pItem)
			Add_Item((CItem*)m_pItem);
	}


	if (m_eColliderType != COLLISION_TYPE::TYPE_END)
	{
		if (m_eColliderType == COLLISION_TYPE::TRIPOD)
			Item_TempModel(m_vColliderPos, m_eColliderType, m_vColliderLook, (CItem*)m_pTripod);
		else
			Item_TempModel(m_vColliderPos, m_eColliderType, m_vColliderLook);

		if (pGameInstance->Is_KeyState(KEY::F, KEY_STATE::TAP))
		{
			if (m_eColliderType == COLLISION_TYPE::TRIPOD)
				Install_Item(m_vColliderPos, m_eColliderType, m_vColliderLook, (CItem*)m_pTripod);
			else
				Install_Item(m_vColliderPos, m_eColliderType, m_vColliderLook);
		}
	}

	m_pRayCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fDist = FLT_MAX;
	m_eColliderType = COLLISION_TYPE::TYPE_END;
	m_vColliderLook = _float4(0.f, 1.f, 0.f, 0.f);
	m_pTripod = nullptr;
	m_pItem = nullptr;

	if (!m_bGrab)
		m_pDoor = nullptr;

	RELEASE_INSTANCE(CGameInstance);
}

void CInventory::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CInventory::Render()
{
	return S_OK;
}

void CInventory::Add_Item(CItem* pItem)
{
	
	if (!pItem->Keep_Item())
	{
		if(nullptr!=m_vInventory[m_iIndex])
			return;
	}
	
	_int	iEmptyIndex = -1;
	for (int i = m_iIndex; i < 3; i=(++i)%3)
	{
		if (m_vInventory[i] == nullptr || m_vInventory[i] == pItem)
		{
			iEmptyIndex = i;
			break;
		}
	}

	if (-1 == iEmptyIndex)
		return;
		
	m_vInventory[iEmptyIndex] = pItem;
	m_vInventory[iEmptyIndex]->Set_Install(false);

	CCollider* pCollider = (CCollider*)m_vInventory[iEmptyIndex]->Get_Component(TEXT("Com_OBB"));
	if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
		static_cast<CVideo_Camera*>(m_vInventory[iEmptyIndex])->Disconnect_Tripod();
	}

	

	if (m_iIndex == iEmptyIndex)
	{
		m_vInventory[m_iIndex]->Set_Enable(true);
		m_vInventory[m_iIndex]->Adjust_Item(m_pPlayerTransform);
	}
	else
		m_vInventory[iEmptyIndex]->Set_Enable(false);

	if (CItem::FLASHLIGHT == pItem->Get_ItemType())
	{
		m_pSpotLight = pItem;
	}
}

void CInventory::Drop_Item()
{
	if (nullptr == m_vInventory[m_iIndex])
		return;

	/*던지는 물리*/
	if (m_pSpotLight == m_vInventory[m_iIndex])
		m_pSpotLight = nullptr;


	m_vInventory[m_iIndex] = nullptr;
}

void CInventory::Install_Item(_float3 _vInstallPos, COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject)
{
	if (nullptr == m_vInventory[m_iIndex])
		return;

	if (m_vInventory[m_iIndex]->Install(_vInstallPos, _eCollisionType, vLook, pConnectObject))
	{
		m_vInventory[m_iIndex]->Set_Install(true);
		m_vInventory[m_iIndex] = nullptr;
	}
}

void CInventory::Change_Item()
{
	if (m_vInventory[m_iIndex])
	{
		CCollider* pCollider = (CCollider*)m_vInventory[m_iIndex]->Get_Component(TEXT("Com_Tripod"));
		if (nullptr != pCollider)
		{
			if (COLLISION_TYPE::TRIPOD == pCollider->Get_Type())
			{
				Drop_Item();
			}
		}
		else
			m_vInventory[m_iIndex]->Change_Item(false);
		
	}
	m_iIndex = (++m_iIndex) % 3;

	
	if (nullptr != m_pSpotLight)
	{
		if (m_vInventory[m_iIndex])
		{
			if (CItem::UVLIGHT == m_vInventory[m_iIndex]->Get_ItemType())
			{
				m_pSpotLight->Set_Enable(false);
			}
			else if (CItem::FLASHLIGHT == m_vInventory[m_iIndex]->Get_ItemType())
			{
				m_pSpotLight->Set_Enable(true);
			}
		}
		else
		{
			m_pSpotLight->Set_Enable(true);
			m_pSpotLight->Change_Item(false);
		}
		
	}
	if (m_vInventory[m_iIndex])
	{
		m_vInventory[m_iIndex]->Change_Item(true);
		m_vInventory[m_iIndex]->Adjust_Item(m_pPlayerTransform);
	}
}

void CInventory::Turn_Switch()
{
	if (m_vInventory[m_iIndex])
		m_vInventory[m_iIndex]->Turn_Switch();

}

void CInventory::Item_TempModel(_float3 _vInstallPos, COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject)
{
	if (m_vInventory[m_iIndex])
		m_vInventory[m_iIndex]->Set_TempModel_Pos(_vInstallPos, _eCollisionType, vLook, pConnectObject);
}

void CInventory::Frequency_Control(_long _lMouseMove)
{
	if(m_vInventory[m_iIndex])
		m_vInventory[m_iIndex]->Frequency_Control(_lMouseMove);
}

HRESULT CInventory::Setup_Component()
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

	return S_OK;
}



void CInventory::On_Collision_Enter(CCollider* pCollider)
{
}

void CInventory::On_Collision_Stay(CCollider* pCollider)
{
	if (COLLISION_TYPE::ITEM == pCollider->Get_Type() || COLLISION_TYPE::THERMOMETER == pCollider->Get_Type() ||
		COLLISION_TYPE::CAMERA == pCollider->Get_Type() || COLLISION_TYPE::SPIRITBOX == pCollider->Get_Type() ||
		COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		_float fCollisionDist = m_pRayCom->Get_Collision_Dist();

		if (DBL_EPSILON < fCollisionDist && m_fDist > fCollisionDist)
		{
			m_fDist = fCollisionDist;
			m_pItem = pCollider->Get_Owner();
		}
	}
	else if (COLLISION_TYPE::OBJECT == pCollider->Get_Type() || COLLISION_TYPE::WALL == pCollider->Get_Type() ||
		COLLISION_TYPE::TRIPOD == pCollider->Get_Type() || COLLISION_TYPE::DOTSPROJECTER == pCollider->Get_Type())
	{
		_float fCollisionDist = m_pRayCom->Get_Collision_Dist();

		if (DBL_EPSILON < fCollisionDist && m_fDist > fCollisionDist)
		{
			m_fDist = fCollisionDist;
			m_vColliderPos = m_pRayCom->Get_CollidePos();
			m_eColliderType = pCollider->Get_Type();
			if (COLLISION_TYPE::WALL == pCollider->Get_Type())
				XMStoreFloat4(&m_vColliderLook, static_cast<CTransform*>(pCollider->Get_Owner()->Get_Component(CGameObject::m_pTransformTag))->Get_State(CTransform::STATE_LOOK));
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

void CInventory::On_Collision_Exit(CCollider* pCollider)
{
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	Safe_Release(m_pRayCom);

}
