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

	m_pTransformCom->Set_Scaled(_float3(1.f, 1.f, 1.f));

	
	return S_OK;
}

void CHandPrint::Tick(_float fTimeDelta)
{
	if (m_pObjectTransform)
	{
		_float3 vScale = m_pTransformCom->Get_Scaled();

		_vector vRight = m_pObjectTransform->Get_State(CTransform::STATE_RIGHT);
		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vLook = m_pObjectTransform->Get_State(CTransform::STATE_LOOK);
		_vector vPos = m_pObjectTransform->Get_State(CTransform::STATE_TRANSLATION);

		vPos = vPos + XMVector3Normalize(vRight) * m_vPosOffSet.x + XMVector3Normalize(vLook) * m_vPosOffSet.z 
			+ XMVector3Normalize(vUp) * m_vPosOffSet.y;

		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook)* vScale.z);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}
}

void CHandPrint::LateTick(_float fTimeDelta)
{
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	GAMEINSTANCE->Add_Object_For_Culling(this, CRenderer::RENDER_DECAL);

	m_pRendererCom->Add_DebugRenderGroup(m_pOBBCom);
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
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_Decals"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_HandPrint"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_OBB*/
	CCollider::COLLIDERDESC  ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(2.f, 2.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.pOwner = this;
	ColliderDesc.m_eObjID = COLLISION_TYPE::DEFAULT;

	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CHandPrint::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4 ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv , XMMatrixTranspose(XMMatrixInverse(nullptr, XMMatrixTranspose(XMLoadFloat4x4(pViewMatrix)))));
	XMStoreFloat4x4(&ProjMatrixInv , XMMatrixTranspose(XMMatrixInverse(nullptr, XMMatrixTranspose(XMLoadFloat4x4(pProjMatrix)))));

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;*/
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_DecalInvWorldMat", &XMMatrixTranspose(XMMatrixInverse(nullptr,m_pTransformCom->Get_WorldMatrix())), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if(FAILED(m_pShaderCom->Set_ShaderResourceView("g_DepthTexture",m_pRendererCom->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	//cube로 받아올 때와 상관이 없음 그냥 있는 큐브 사용하는게 나을 듯 한데 혹시 모르니까

	return S_OK;
}

void CHandPrint::Set_Position(CTransform* _pTransform,_vector vPlayerPos)
{
	m_pObjectTransform = _pTransform;

	_vector vRight = _pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vPos = _pTransform->Get_State(CTransform::STATE_TRANSLATION);

	_float4 vPlayerDir;
	XMStoreFloat4(&vPlayerDir, vPlayerPos - vPos);
	_vector vEraseHeight = XMVectorSet(vPlayerDir.x, 0.f, vPlayerDir.z, 0.f);

	if (0.f > XMVectorGetX(XMVector3Dot(XMVector3Normalize(vRight), vEraseHeight)))
	{//문의 기본 오프셋이 왼쪽으로 가있지만 1,0,0,0이 right인 상황
		m_vPosOffSet.x = vPlayerDir.x;
		if (0.f > XMVectorGetX(XMVector3Length(XMVector3Cross(vRight, vEraseHeight))))
		{
			m_vPosOffSet.z = 1.f;
		}
		else
		{
			m_vPosOffSet.z = -1.f;
		}
		m_vPosOffSet.y = 10.f;
	}

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
	Safe_Release(m_pOBBCom);

}
