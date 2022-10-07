#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "PipeLine.h"
#include "GameInstance.h"
#include "RenderTarget.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

CRenderer::CRenderer(const CRenderer& rhs)
	:CComponent(rhs)
	,m_pTarget_Manager(rhs.m_pTarget_Manager)
	,m_pLight_Manager(rhs.m_pLight_Manager)
	
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
	
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (eGroup >= RENDER_END ||
		nullptr == pGameObject)
		return E_FAIL;

	m_RenderObjects[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* 렌더타겟들을 추가한다. */
	GAMEINSTANCE->Add_Renderer(CRenderer_Manager::RENDERER_STATIC, this);
	GAMEINSTANCE->Add_ItemFrustum(CFrustum::FRUSTUM_STATIC, this, nullptr);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);


	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	///* For.Target_Emissive*/
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Emissive"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
	//	return E_FAIL;


	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* For.MRT_Deferred */
	/* 조명연산이 필요한 객체들을 그릴 때, 추후 빛연산에 필요한 정보를 저장받기위한 렌더타겟들 (Diffuse + Normal ) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Specular"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	/* 조명연산한 결과를 저장해놓기위한 타겟 (Shade + Specular) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;


#ifdef _DEBUG	
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Emissive"), 300.f, 500.f, 200.f, 200.f)))
		return E_FAIL;*/
#endif // _DEBUG

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

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Begin_RenderTarget(const _tchar* _pMRTTag)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, _pMRTTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::End_RenderTarget()
{
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Set_Environment()
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	GAMEINSTANCE->Clear_DepthStencil_View();

	m_CamViewMat = *(pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_VIEW));
	m_CamProjMat = *(pPipeLine->Get_Transform_TP(CPipeLine::D3DTS_PROJ));
	m_vCamPosition = pPipeLine->Get_CamPosition();


	XMStoreFloat4x4(&m_CamViewInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW))));
	XMStoreFloat4x4(&m_CamProjInv, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ))));
	
	RELEASE_INSTANCE(CPipeLine);

	m_pTarget_Manager->Clear_MRT(TEXT("MRT_Deferred"));
	m_pTarget_Manager->Clear_MRT(TEXT("MRT_LightAcc"));

}

HRESULT CRenderer::Draw_RenderGroup()
{
	Set_Environment();

	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // _DEBUG

	End_Environment();

	return S_OK;
}

void CRenderer::Draw_On_Texture(CRenderTarget* pRenderTarget, CTexture* pTexture, CShader* pShader,_int iPassindex,const _tchar* pText, _float2 vRenderPos, const _tchar* pTexttag)
{

	GAMEINSTANCE->Clear_DepthStencil_View();

	D3D11_TEXTURE2D_DESC TextureDesc;
	pRenderTarget->Get_Desc(&TextureDesc);

	_matrix WorldMat = XMMatrixIdentity();
	WorldMat.r[0] = XMVectorSet(TextureDesc.Width, 0.f, 0.f, 0.f);
	WorldMat.r[1] = XMVectorSet(0.f, TextureDesc.Height, 0.f, 0.f);

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(WorldMat));

	_float4x4 ProjMatrix;
	XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(TextureDesc.Width, TextureDesc.Height, 0.f, 1.f)));


	m_pTarget_Manager->Store_BackBuffer(m_pContext);//백버퍼 보관해놓음

	m_pTarget_Manager->Set_RenderTarget(m_pContext, pRenderTarget);

	pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	pShader->Set_RawValue("g_ProjMatrix", &ProjMatrix, sizeof(_float4x4));

	if (FAILED(pTexture->Set_ShaderResourceView(pShader,"g_DiffuseTexture", iPassindex)))
		return;

	pShader->Begin(iPassindex);

	m_pVIBuffer->Render();
	//const _tchar* pString, _fvector vPosition, _fvector vColor, float rotation, _fvector vOrigin, _vector vScale

	GAMEINSTANCE->Render_Font(pTexttag, pText, XMVectorSet(vRenderPos.x, vRenderPos.y,0.f,0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMConvertToRadians(90.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 0.f));

	m_pTarget_Manager->Set_BackBuffer(m_pContext);//작업 끝났으므로 백버퍼 다시 세팅
}

void CRenderer::Draw_On_Texture(CRenderTarget* pRenderTarget, CTexture* pTexture[], CShader* pShader, _int iPassindex, _float3 vRenderPos[])
{
}

HRESULT CRenderer::Render_Priority()
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

HRESULT CRenderer::Render_NonAlphaBlend()
{
	/* 렌더타겟을 장치에 Diffuse + Normal 바인딩한ㄷ앋. */
	if (FAILED(Begin_RenderTarget( TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (int i = RENDER_TERRAIN; i < RENDER_NONLIGHT; ++i)
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

HRESULT CRenderer::Render_Lights()
{
	/* 셰이드 타겟을 장치에 바인드한다. */
	if (FAILED(Begin_RenderTarget( TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* 모든 빛은 이 노멀텍스쳐(타겟)과 연산이 이뤄지면 되기때문에. 
	모든 빛마다 각각 던질피룡가 없다. */
	if (FAILED(m_pShader->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
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

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pShader->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;

	/*if (FAILED(m_pShader->Set_ShaderResourceView("g_EmissiveTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Emissive")))))
		return E_FAIL;*/

	/* 모든 빛들은 셰이드 타겟을 꽉 채우고 지굑투영으로 그려지면 되기때문에 빛마다 다른 상태를 줄 필요가 없다. */
	m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	m_pShader->Begin(4);

	/* 사각형 버퍼를 백버퍼위에 그려낸다. */
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
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

HRESULT CRenderer::Render_AlphaBlend()
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

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}
#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	for (auto& pComponent : m_DebugComponents)
	{
		/*if (nullptr != pComponent)*/
			pComponent->Render();

		Safe_Release(pComponent);
	}

	m_DebugComponents.clear();

	//if (nullptr == m_pTarget_Manager)
	//	return E_FAIL;

	//if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
	//	return E_FAIL;

	//if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
	//	return E_FAIL;

	//m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer);	
	//m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer);

	return S_OK;
}

HRESULT CRenderer::Add_DebugRenderGroup(CComponent * pComponent)
{
	m_DebugComponents.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}


#endif // _DEBUG

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*		pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;	
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderObjects[i])
			Safe_Release(pGameObject);

		m_RenderObjects[i].clear();
	}
#ifdef _DEBUG
	for (auto& pComponent : m_DebugComponents)
		Safe_Release(pComponent);

	m_DebugComponents.clear();

#endif
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
}
