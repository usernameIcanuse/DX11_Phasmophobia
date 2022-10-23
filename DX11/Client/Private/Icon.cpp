#include "stdafx.h"
#include "..\Public\Icon.h"

#include "GameInstance.h"

CIcon::CIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CIcon::CIcon(const CIcon& rhs)
	: CGameObject(rhs)
{
}

HRESULT CIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIcon::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	
  	return S_OK;
}

void CIcon::Tick(_float fTimeDelta)
{

	m_bSelected = false;
	m_bOnMouse = false;

	if (false == m_bLock)
	{
		_float4 fPosition;
		if (CMath_Utility::Picking(m_pVIBufferCom,m_pTransformCom,&fPosition))
		{
			m_bOnMouse = true;
			if (CGameInstance::Get_Instance()->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
			{
				m_bSelected = true;
			}
		}
	}
	
}

void CIcon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CIcon::Render()
{	
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CIcon::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
}

void CIcon::Set_Transform(_matrix WorldMat)
{
	m_pTransformCom->Set_WorldMatrix(WorldMat);
}

void CIcon::Set_Texture(_uint iLevelIndex,const _tchar* _pPrototypeTag)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(iLevelIndex, _pPrototypeTag, TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return;
}

HRESULT CIcon::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CIcon::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("bAlpha", &m_bOnMouse, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

CIcon * CIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CIcon*		pInstance = new CIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CIcon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CIcon::Clone(void * pArg)
{
	CIcon*		pInstance = new CIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
