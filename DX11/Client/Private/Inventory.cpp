#include "stdafx.h"
#include "Inventory.h"
#include "GameInstance.h"


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
		m_pPlayer = (CGameObject*)pArg;
	}

	m_vInventory.reserve(3);

	for (int i = 0; i < 3; ++i)
	{
		CGameObject* pNull = nullptr;
		m_vInventory.push_back(pNull);
	}
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;




	return S_OK;
}

void CInventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_vInventory[m_iIndex])
	{

		CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(CGameObject::m_pTransformTag);
		_vector     vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

		_vector     vRight = pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = pPlayerTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

		vPlayerPos += XMVector3Normalize(vRight);
		vPlayerPos -= XMVector3Normalize(vUp);
		vPlayerPos += vLook * 4;

		CTransform* pItemTransform = (CTransform*)m_vInventory[m_iIndex]->Get_Component(CGameObject::m_pTransformTag);
		pItemTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerPos);
		pItemTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		pItemTransform->Set_State(CTransform::STATE_UP, vUp);
		pItemTransform->Set_State(CTransform::STATE_LOOK, vLook);
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
		
	
	CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(CGameObject::m_pTransformTag);
	_vector     vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

	_vector     vRight = pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector		vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

	vPlayerPos += XMVector3Normalize(vRight);
	vPlayerPos -= XMVector3Normalize(vUp);
	vPlayerPos += vLook * 4;

	CTransform* pItemTransform = (CTransform*)pItem->Get_Component(CGameObject::m_pTransformTag);
	pItemTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerPos);
	pItemTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	pItemTransform->Set_State(CTransform::STATE_UP, vUp);
	pItemTransform->Set_State(CTransform::STATE_LOOK, vLook);

	m_vInventory[iEmptyIndex] = pItem;

	if (m_iIndex == iEmptyIndex)
		m_vInventory[m_iIndex]->Set_Enable(true);
	else
		m_vInventory[iEmptyIndex]->Set_Enable(false);
}

void CInventory::Drop_Item()
{
	if (nullptr == m_vInventory[m_iIndex])
		return;

	m_vInventory[m_iIndex] = nullptr;
}

void CInventory::Install_Item(_float3 _vInstallPos)
{
	if (nullptr == m_vInventory[m_iIndex]/* || 설치 불가 아이템*/)
		return;

	CGameObject* pCurItem = m_vInventory[m_iIndex];
	CTransform* pItemTransform = (CTransform*)pCurItem->Get_Component(CGameObject::m_pTransformTag);

	pItemTransform->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&_vInstallPos), 1.f));

	m_vInventory[m_iIndex] = nullptr;
}

void CInventory::Change_Item()
{
	if (m_vInventory[m_iIndex])
		m_vInventory[m_iIndex]->Set_Enable(false);

	m_iIndex = (++m_iIndex) % 3;

	if (m_vInventory[m_iIndex])
	{
		m_vInventory[m_iIndex]->Set_Enable(true);

		CTransform* pPlayerTransform = (CTransform*)m_pPlayer->Get_Component(CGameObject::m_pTransformTag);
		_vector     vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

		_vector     vRight = pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
		_vector		vUp = pPlayerTransform->Get_State(CTransform::STATE_UP);
		_vector		vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);

		vPlayerPos += XMVector3Normalize(vRight);
		vPlayerPos -= XMVector3Normalize(vUp);
		vPlayerPos += vLook * 4;

		CTransform* pItemTransform = (CTransform*)m_vInventory[m_iIndex]->Get_Component(CGameObject::m_pTransformTag);
		pItemTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerPos);
		pItemTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		pItemTransform->Set_State(CTransform::STATE_UP, vUp);
		pItemTransform->Set_State(CTransform::STATE_LOOK, vLook);
	}
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
