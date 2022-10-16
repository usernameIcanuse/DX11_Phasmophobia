#include "stdafx.h"
#include "..\Public\UIIcon.h"

#include "GameInstance.h"

CUIIcon::CUIIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CUIIcon::CUIIcon(const CUIIcon& rhs)
	: CGameObject(rhs)
{
}

HRESULT CUIIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIIcon::Initialize(void * pArg)
{


	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	
  	return S_OK;
}

void CUIIcon::Tick(_float fTimeDelta)
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

void CUIIcon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CUIIcon::Render()
{	
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUIIcon::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
}

void CUIIcon::Set_Transform(_matrix WorldMat)
{
	m_pTransformCom->Set_WorldMatrix(WorldMat);
}

void CUIIcon::Set_Texture(const _tchar* _pPrototypeTag)
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, _pPrototypeTag, TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return;
}

HRESULT CUIIcon::SetUp_Components()
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

HRESULT CUIIcon::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

CUIIcon * CUIIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIIcon*		pInstance = new CUIIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIIcon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUIIcon::Clone(void * pArg)
{
	CUIIcon*		pInstance = new CUIIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
