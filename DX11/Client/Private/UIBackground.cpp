#include "stdafx.h"
#include "..\Public\UIBackground.h"
#include "GameInstance.h"
#include "UIIcon.h"

CUIBackground::CUIBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CUIBackground::CUIBackground(const CUIBackground& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUIBackground::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIBackground::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


  	return S_OK;
}

void CUIBackground::Tick(_float fTimeDelta)
{
	m_iSelectedMenu = 0;
	
	for (int i = 0; i < m_vecUIIcon.size(); ++i)
	{
		if (static_cast<CUIIcon*>(m_vecUIIcon[i])->Selected())
			m_iSelectedMenu = i + 1;
	}
}

void CUIBackground::LateTick(_float fTimeDelta)
{
	
}

HRESULT CUIBackground::Render()
{
	return S_OK;
}

void CUIBackground::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for(auto& elem : m_vecUIIcon)
		elem->Set_Enable(_bEnable);
}

void CUIBackground::Icon_Lock(_bool _bLock)
{
	for (auto& elem : m_vecUIIcon)
	{
		elem->Set_Lock(_bLock);
	}
}

CUIBackground * CUIBackground::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIBackground*		pInstance = new CUIBackground(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIBackground");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUIBackground::Clone(void * pArg)
{
	CUIBackground*		pInstance = new CUIBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIBackground::Free()
{
	__super::Free();

}
