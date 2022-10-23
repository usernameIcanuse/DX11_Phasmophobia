#include "stdafx.h"
#include "..\Public\Journal.h"
#include "GameInstance.h"
#include "UIIcon.h"
#include "Main.h"
#include "Evidence.h"

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
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Setup_UI()))
		return E_FAIL;

  	return S_OK;
}

void CJournal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_int iSelectedIndex = Selected_Menu();

	switch (iSelectedIndex)
	{
	case 1:
		m_pMain->Set_Enable(true);
		m_pEvidence->Set_Enable(false);
		break;

	case 2:
		m_pMain->Set_Enable(false);
		m_pEvidence->Set_Enable(true);
		break;

	case 3:
		break;
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

void	CJournal::Main_On(_bool _bEnable)
{
	m_pMain->Set_Enable(_bEnable);
}

void	CJournal::Evidence_On(_bool _bEnable)
{
	m_pEvidence->Set_Enable(_bEnable);
}

HRESULT CJournal::Setup_UI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	/*For. Main*/
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UI"), TEXT("Prototype_GameObject_UIMain"), (CGameObject**)&m_pMain)))
		return E_FAIL;

	m_pMain->Set_Enable(false);
	/*For. Evidence*/
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UI"), TEXT("Prototype_GameObject_UIEvidence"), (CGameObject**)&m_pEvidence)))
		return E_FAIL;
	m_pEvidence->Set_Enable(false);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CJournal::Setup_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUIIcon* pIcon = nullptr;

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//¸Þ´º
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Icon"));
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
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
