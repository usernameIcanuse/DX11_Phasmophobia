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

	if (FAILED(Setup_Icon()))
		return E_FAIL;

  	return S_OK;
}

void CJournal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	switch (m_iSelectedMenu)
	{
	case 1:
		m_pMain->Set_Enable(true);
		m_pEvidence->Set_Enable(false);
		break;

	case 2:
		
		break;

	case 3:
		m_pMain->Set_Enable(false);
		m_pEvidence->Set_Enable(true);
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

void	CJournal::Off_AllUI()
{
	if (true == m_pMain->Get_Enable())
	{
		m_pMain->Set_Enable(false);
	}

	if (true == m_pEvidence->Get_Enable())
	{
		m_pEvidence->Set_Enable(false);
	}
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

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//메뉴
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Icon"));
	pIcon->Set_IconPosition(330, 55, 200, 40);
	pIcon->Set_PassIndex(3);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//사진
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Icon"));
	pIcon->Set_IconPosition(750, 57, 200, 40);
	pIcon->Set_PassIndex(3);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//증거
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Icon"));
	pIcon->Set_IconPosition(950, 57, 200, 40);
	pIcon->Set_PassIndex(3);
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
