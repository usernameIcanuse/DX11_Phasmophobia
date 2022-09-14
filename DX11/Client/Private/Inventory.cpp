#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"
#include "Video_Camera.h"


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

	/* ax + by + cz + d =0*/
	//(a,b,c,d) = XMPlaneFromPoints(p1,p2,p3)

	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_vInventory[m_iIndex])
	{
		Adjust_Item(m_vInventory[m_iIndex]);
	}
}

void CInventory::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CInventory::Render()
{
	return S_OK;
}

void CInventory::Add_Item(CGameObject* pItem)
{
	_int	iEmptyIndex = -1;
	for (int i = m_iIndex; i < 3; i=(++i)%3)
	{
		if (m_vInventory[i] == nullptr)
		{
			iEmptyIndex = i;
			break;
		}
	}

	if (-1 == iEmptyIndex)
		return;
		
	m_vInventory[iEmptyIndex] = (CItem*)pItem;


	CCollider* pCollider = (CCollider*)m_vInventory[iEmptyIndex]->Get_Component(TEXT("Com_OBB"));
	if (COLLISION_TYPE::CAMERA == pCollider->Get_Type())
	{
		static_cast<CVideo_Camera*>(m_vInventory[iEmptyIndex])->Disconnect_Tripod();
	}

	

	if (m_iIndex == iEmptyIndex)
	{
		m_vInventory[m_iIndex]->Set_Enable(true);
		Adjust_Item(m_vInventory[m_iIndex]);
	}
	else
		m_vInventory[iEmptyIndex]->Set_Enable(false);
}

void CInventory::Drop_Item()
{
	if (nullptr == m_vInventory[m_iIndex])
		return;

	/*던지는 물리*/
	
	m_vInventory[m_iIndex] = nullptr;
}

void CInventory::Install_Item(_float3 _vInstallPos, COLLISION_TYPE _eCollisionType, _float4 vLook, CItem* pConnectObject)
{
	if (nullptr == m_vInventory[m_iIndex])
		return;

	if(m_vInventory[m_iIndex]->Install(_vInstallPos,_eCollisionType,vLook, pConnectObject))
		m_vInventory[m_iIndex] = nullptr;

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
			m_vInventory[m_iIndex]->Set_Enable(false);
	}
	m_iIndex = (++m_iIndex) % 3;

	if (m_vInventory[m_iIndex])
	{
		Adjust_Item(m_vInventory[m_iIndex]);
	}
}

void CInventory::Turn_Switch()
{
	if (m_vInventory[m_iIndex])
		m_vInventory[m_iIndex]->Turn_Switch();

}

void	CInventory::Adjust_Item(CItem* pItem)
{
	pItem->Set_Enable(true);

	_vector     vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector     vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	_float3		vAdjustPos = pItem->Get_AdjustPos();

	vPlayerPos += XMVector3Normalize(vRight) * vAdjustPos.x;
	vPlayerPos -= XMVector3Normalize(vUp)* vAdjustPos.y;
	vPlayerPos += vLook* vAdjustPos.z;

	CTransform* pItemTransform = (CTransform*)pItem->Get_Component(CGameObject::m_pTransformTag);
	pItemTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerPos);
	pItemTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	pItemTransform->Set_State(CTransform::STATE_UP, vUp);
	pItemTransform->Set_State(CTransform::STATE_LOOK, vLook);

	pItem->Update_Collider();
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

}
