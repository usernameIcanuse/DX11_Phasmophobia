#include "stdafx.h"
#include "..\Public\HandPrint.h"

#include "GameInstance.h"

CHandPrint::CHandPrint(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CHandPrint::CHandPrint(const CHandPrint & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHandPrint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHandPrint::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	return S_OK;
}

void CHandPrint::Tick(_float fTimeDelta)
{
	
}

void CHandPrint::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_DECAL);
}

HRESULT CHandPrint::Render()
{
	
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CHandPrint::SetUp_Components()
{


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Decals"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_HandPrint"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHandPrint::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

void CHandPrint::Set_Position(CTransform* _pTransform)
{
	_vector vLook = _pTransform->Get_State(CTransform::STATE_LOOK);
	_vector vRight = _pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vPos = _pTransform->Get_State(CTransform::STATE_TRANSLATION);
	vPos += XMVector3Normalize(vLook) * 0.3f;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

}

CHandPrint * CHandPrint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHandPrint*		pInstance = new CHandPrint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHandPrint");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CHandPrint::Clone(void * pArg)
{
	CHandPrint*		pInstance = new CHandPrint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHandPrint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHandPrint::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
