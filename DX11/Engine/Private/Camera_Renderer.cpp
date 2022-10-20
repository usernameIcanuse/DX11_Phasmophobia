#include "..\Public\Camera_Renderer.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "RenderTarget.h"


CCamera_Renderer::CCamera_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CRenderer(pDevice, pContext)
{
}

CCamera_Renderer::CCamera_Renderer(const CCamera_Renderer& rhs)
	:CRenderer(rhs)
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

	m_pRenderScreen = CRenderTarget::Create(m_pDevice, m_pContext, ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f));
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

	m_pShaderPostProcess = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Shaderfiles/Shader_PostProcessing.hlsl"), VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaderPostProcess)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Renderer::Draw_RenderGroup(_float fTimeDelta)
{
	Set_Environment();

	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_Decal()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;
	if (FAILED(Post_Processing(fTimeDelta)))
		return E_FAIL;

	End_Environment();

	return S_OK;
}

HRESULT CCamera_Renderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CCamera_Renderer::Render_NonAlphaBlend()
{
	/* 렌더타겟을 장치에 Diffuse + Normal 바인딩한ㄷ앋. */
	if (FAILED(Begin_RenderTarget(TEXT("MRT_Deferred"))))
		return E_FAIL;


	for (int i = RENDER_TERRAIN; i < RENDER_DECAL; ++i)
	{
		for (auto& pGameObject : m_RenderObjects[i])
		{
			if (nullptr != pGameObject)
			{
				if (FAILED(pGameObject->SetUp_ShaderResource(&m_CamViewMat, &m_CamProjMat)))
				{
					Safe_Release(pGameObject);
					continue;
				}
				pGameObject->Render();
			}
			Safe_Release(pGameObject);
		}
		m_RenderObjects[i].clear();
	}

	if (FAILED(End_RenderTarget()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Renderer::Render_Decal()
{
	if (FAILED(Begin_RenderTarget(TEXT("MRT_Decals"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RENDER_DECAL])
	{
		if (FAILED(pGameObject->SetUp_ShaderResource(&m_CamViewMat, &m_CamProjMat)))
		{
			Safe_Release(pGameObject);
			continue;
		}
		pGameObject->Render();
		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_DECAL].clear();

	if (FAILED(End_RenderTarget()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamera_Renderer::Render_Lights()
{
	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(Begin_RenderTarget(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에.
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_pShader->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_UVLightTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Decals")))))
		return E_FAIL;


	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_ViewMatrixInv", &m_CamViewInv, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrixInv", &m_CamProjInv, sizeof(_float4x4));

	m_pShader->Set_RawValue("g_vCamPosition", &m_vCamPosition, sizeof(_float4));


	m_pLight_Manager->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(End_RenderTarget()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Renderer::Render_Blend()
{
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;


	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Begin(5);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CCamera_Renderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->SetUp_ShaderResource(&m_CamViewMat, &m_CamProjMat)))
			{
				Safe_Release(pGameObject);
				continue;
			}
			pGameObject->Render();
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CCamera_Renderer::Render_AlphaBlend()
{
	m_RenderObjects[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

	for (auto& pGameObject : m_RenderObjects[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->SetUp_ShaderResource(&m_CamViewMat, &m_CamProjMat)))
			{
				Safe_Release(pGameObject);
				continue;
			}
			pGameObject->Render();
		}
		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CCamera_Renderer::Post_Processing(_float fTimeDelta)
{
	m_pTarget_Manager->End_MRT_For_Texture(m_pContext, m_pCameraScreen);

	if (FAILED(m_pShaderPostProcess->Set_ShaderResourceView("g_Texture", m_pRenderScreen->Get_SRV())))
		return E_FAIL;
	static _float fTimeAcc = 0.f;
	fTimeAcc += fTimeDelta;
	if (fTimeAcc > 0.05f)
	{
		fTimeAcc = 0.f;
		m_fTimeDelta = fTimeDelta;
	}
	if (FAILED(m_pShaderPostProcess->Set_RawValue("g_Time", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	m_pShaderPostProcess->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShaderPostProcess->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderPostProcess->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShaderPostProcess->Begin(m_iPassIndex);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pVIBuffer->Render();

	return S_OK;
}


ID3D11ShaderResourceView* CCamera_Renderer::Get_SRV()
{
	return m_pCameraScreen->Get_SRV();
}

void CCamera_Renderer::Post_Processing_Pass(_uint iPassIndex)
{
	m_iPassIndex = iPassIndex;
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
	if (FAILED(m_pTarget_Manager->End_MRT_For_Texture(m_pContext, m_pRenderScreen)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Renderer::Set_Environment()
{
	m_pTarget_Manager->Store_BackBuffer(m_pContext);

	m_pRenderScreen->Clear();
	m_pCameraScreen->Clear();
	GAMEINSTANCE->Clear_DepthStencil_View();

	if (FAILED(m_pTarget_Manager->End_MRT_For_Texture(m_pContext, m_pRenderScreen)))
		return;

	_matrix		ViewMatrix= m_pOwnerTransform->Get_WorldMatrix();
	ViewMatrix.r[0] = XMVector3Normalize(ViewMatrix.r[0]);
	ViewMatrix.r[1] = XMVector3Normalize(ViewMatrix.r[1]);
	ViewMatrix.r[2] = XMVector3Normalize(ViewMatrix.r[2]);
	ViewMatrix.r[3] += ViewMatrix.r[1]*0.25 + ViewMatrix.r[2]*0.5f;

	XMStoreFloat4(&m_vCamPosition, ViewMatrix.r[3]);

	XMStoreFloat4x4(&m_CamViewMat,XMMatrixTranspose(XMMatrixInverse(nullptr, ViewMatrix)));
	m_CamProjMat = *(GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ));

	XMStoreFloat4x4(&m_CamViewInv, XMMatrixTranspose(ViewMatrix));
	XMStoreFloat4x4(&m_CamProjInv, XMMatrixTranspose(XMMatrixInverse(nullptr, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ))));

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height, 0.f, 0.f);

	m_pTarget_Manager->Clear_MRT(TEXT("MRT_Deferred"));
	m_pTarget_Manager->Clear_MRT(TEXT("MRT_LightAcc"));
	m_pTarget_Manager->Clear_MRT(TEXT("MRT_Decals"));
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
	Safe_Release(m_pRenderScreen);
	Safe_Release(m_pOwnerTransform);

	Safe_Release(m_pShaderPostProcess);
}
