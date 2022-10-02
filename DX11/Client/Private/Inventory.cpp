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
		m_vInventory[m_iIndex]->Adjust_Item(m_pPlayerTransform);
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
		if (m_vInventory[i] == nullptr)
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
			m_vInventory[m_iIndex]->Change_Item();
		
	}
	m_iIndex = (++m_iIndex) % 3;

	if (m_vInventory[m_iIndex])
	{
		if (nullptr != m_pSpotLight)
		{
			if (CItem::UVLIGHT == m_vInventory[m_iIndex]->Get_ItemType())
			{
				m_pSpotLight->Set_Enable(false);
			}
			else
				m_pSpotLight->Set_Enable(true);
		}

		m_vInventory[m_iIndex]->Change_Item();
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
