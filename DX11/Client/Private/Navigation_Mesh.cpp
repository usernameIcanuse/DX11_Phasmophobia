#include "stdafx.h"
#include "..\Public\Navigation_Mesh.h"

#include "GameInstance.h"

CNavigation_Mesh::CNavigation_Mesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CNavigation_Mesh::CNavigation_Mesh(const CNavigation_Mesh & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNavigation_Mesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNavigation_Mesh::Initialize(void * pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

void CNavigation_Mesh::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CNavigation_Mesh::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	//m_pRendererCom->Add_DebugRenderGroup(m_pNavigationCom);
#endif
}

HRESULT CNavigation_Mesh::Render()
{
	return S_OK;
}

HRESULT CNavigation_Mesh::SetUp_Components()
{
#ifdef _DEBUG
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
#endif

	/* For.Com_Navigation*/
	CNavigation::NAVIDESC	NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));
	NaviDesc.m_iCurrentIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation_Ghost"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	return S_OK;
}


CNavigation_Mesh * CNavigation_Mesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNavigation_Mesh*		pInstance = new CNavigation_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNavigation_Mesh");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CNavigation_Mesh::Clone(void * pArg)
{
	CNavigation_Mesh*		pInstance = new CNavigation_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation_Mesh::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif
	
	Safe_Release(m_pNavigationCom);

}
