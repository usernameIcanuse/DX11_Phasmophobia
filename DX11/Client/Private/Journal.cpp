#include "stdafx.h"
#include "..\Public\Journal.h"
#include "GameInstance.h"
#include "UIIcon.h"

CJournal::CJournal(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CJournal::CJournal(const CJournal& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CJournal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CJournal::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


  	return S_OK;
}

void CJournal::Tick(_float fTimeDelta)
{
	m_iSelectedMenu = 0;
	
	for (int i = 0; i < m_vecUIIcon.size(); ++i)
	{
		if (static_cast<CUIIcon*>(m_vecUIIcon[i])->Selected())
			m_iSelectedMenu = i + 1;
	}
}

void CJournal::LateTick(_float fTimeDelta)
{
	
}

HRESULT CJournal::Render()
{
	return S_OK;
}

void CJournal::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for(auto& elem : m_vecUIIcon)
		elem->Set_Enable(_bEnable);
}

void CJournal::Icon_Lock(_bool _bLock)
{
	for (auto& elem : m_vecUIIcon)
	{
		elem->Set_Lock(_bLock);
	}
}

CJournal * CJournal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CJournal*		pInstance = new CJournal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CJournal");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CJournal::Clone(void * pArg)
{
	CJournal*		pInstance = new CJournal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CJournal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CJournal::Free()
{
	__super::Free();

}
