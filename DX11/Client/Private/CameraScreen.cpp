#include "stdafx.h"
#include "..\Public\CameraScreen.h"
#include "GameInstance.h"

CCamera_Screen::CCamera_Screen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CItem(pDevice, pContext)
{
}

CCamera_Screen::CCamera_Screen(const CCamera_Screen & rhs)
	: CItem(rhs)
{
}

HRESULT CCamera_Screen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Screen::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	
	if (nullptr != pArg)//사용하는 카메라의 트랜스폼을 가져옴
	{
		m_pCameraTransform = (CTransform*)pArg;
		Safe_AddRef(m_pCameraTransform);
	}

	if (FAILED(Setup_Component()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(300.f, 0.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 225.f, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(0.f,250.f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	return S_OK;
}

void CCamera_Screen::Tick(_float fTimeDelta)
{
	/*매 틱마다 카메라 화면 위치로 조정해주기*/
	if (m_bSwitch)
	{
		GAMEINSTANCE->Add_Renderer(CRenderer_Manager::VIDEO_CAMERA, m_pCameraRenderer);
		GAMEINSTANCE->Add_ItemFrustum(CFrustum::FRUSTUM_ITEM, (CRenderer*)m_pCameraRenderer, m_pCameraTransform);
	}
}

void CCamera_Screen::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCamera_Screen::Render()
{
	if (FAILED(SetUp_ShaderResource(nullptr, nullptr)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCamera_Screen::Setup_Component()
{
	/* For.Com_Shader */
	/*if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Shader_VtxNorTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;//나중에 norrect로 바꿔야함

	/* For.Com_CameraRenderer*/
	if (FAILED(__super::Add_Component(LEVEL_STAGE1, TEXT("Prototype_Component_Camera_Renderer"), TEXT("Com_CameraRenderer"), (CComponent**)&m_pCameraRenderer,m_pCameraTransform)))
		return E_FAIL;

	/*For.Com_Renderer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
		
	return S_OK;
}

HRESULT CCamera_Screen::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/*if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pProjMatrix, sizeof(_float4x4))))
		return E_FAIL;*/

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pCameraRenderer->Get_SRV())))
		return E_FAIL;


	return S_OK;
}

void CCamera_Screen::Turn_Switch()
{
	m_bSwitch = !m_bSwitch;
	if (false == m_bSwitch)
		m_pCameraRenderer->Clear_RenderTarget();
}

CCamera_Screen * CCamera_Screen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Screen*		pInstance = new CCamera_Screen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Screen");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CCamera_Screen::Clone(void * pArg)
{
	CCamera_Screen*		pInstance = new CCamera_Screen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Screen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Screen::Free()
{
	__super::Free();

	Safe_Release(m_pCameraRenderer);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCameraTransform);
	

}
