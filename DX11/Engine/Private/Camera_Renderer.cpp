#include "..\Public\Camera_Renderer.h"
#include "GameInstance.h"
#include "RenderTarget.h"


CCamera_Renderer::CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CRenderer(pDevice, pContext)
{
}

HRESULT CCamera_Renderer::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CCamera_Renderer::Initialize(void* pArg)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	m_pCameraScreen = CRenderTarget::Create(m_pDevice, m_pContext, ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f,0.f,0.f,1.f));
	
	if (nullptr != pArg)
	{
		m_pOwnerTransform = (CTransform*)pArg;
		Safe_AddRef(m_pOwnerTransform);
	}

	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.f, 1.f)));

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

ID3D11ShaderResourceView* CCamera_Renderer::Get_SRV()
{
	return m_pCameraScreen->Get_SRV();
}

void CCamera_Renderer::Clear_RenderTarget()
{
	if (m_pCameraScreen)
		m_pCameraScreen->Clear();
}

HRESULT CCamera_Renderer::Begin_RenderTarget(const _tchar* _pMRTTag)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_For_Texture(m_pContext, _pMRTTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Renderer::End_RenderTarget()
{
	if (FAILED(m_pTarget_Manager->End_MRT_For_Texture(m_pContext, m_pCameraScreen)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Renderer::Set_Environment()
{
	m_pTarget_Manager->Store_BackBuffer(m_pContext);

	m_pCameraScreen->Clear();
	GAMEINSTANCE->Clear_DepthStencil_View();

	if (FAILED(m_pTarget_Manager->End_MRT_For_Texture(m_pContext, m_pCameraScreen)))
		return;

	_matrix		ViewMatrix= m_pOwnerTransform->Get_WorldMatrix();
	ViewMatrix.r[0] = XMVector3Normalize(ViewMatrix.r[0]);
	ViewMatrix.r[1] = XMVector3Normalize(ViewMatrix.r[1]);
	ViewMatrix.r[2] = XMVector3Normalize(ViewMatrix.r[2]);

	XMStoreFloat4(&m_vCamPosition, ViewMatrix.r[3]);

	XMStoreFloat4x4(&m_CamViewMat,XMMatrixTranspose(XMMatrixInverse(nullptr, ViewMatrix)));
	m_CamProjMat = *(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ));

	XMStoreFloat4x4(&m_CamViewInv, XMMatrixTranspose(ViewMatrix));
	XMStoreFloat4x4(&m_CamProjInv, XMMatrixTranspose(XMMatrixInverse(nullptr, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ))));

}

void CCamera_Renderer::End_Environment()
{
	m_pTarget_Manager->Set_BackBuffer(m_pContext);
}

CCamera_Renderer * CCamera_Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Renderer*		pInstance = new CCamera_Renderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCamera_Renderer::Clone(void * pArg)
{
	CCamera_Renderer* pInstance = new CCamera_Renderer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CCamera_Renderer::Free()
{
	__super::Free();

	Safe_Release(m_pCameraScreen);
	Safe_Release(m_pOwnerTransform);
}
