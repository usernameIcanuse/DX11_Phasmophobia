#include "stdafx.h"
#include "..\Public\Lobby_GameResult.h"
#include "GameInstance.h"
#include "UIIcon.h"


CLobby_GameResult::CLobby_GameResult(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CLobby_GameResult::CLobby_GameResult(const CLobby_GameResult& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CLobby_GameResult::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_GameResult::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pTransformCom->Set_Scaled(_float3(12.5f, 9.2f, 1.f));

		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vPos = XMLoadFloat4((_float4*)pArg) - vLook * 0.3f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_Icon()))
		return E_FAIL;

	STAGEDESC tDesc = *GAMEINSTANCE->Get_StageDesc();

	m_iTotalMinute = (_int)tDesc.fTotalPlayTime / 60;
	m_iTotalSecond = (_int)tDesc.fTotalPlayTime % 60;

	m_iHouseMinute = (_int)tDesc.fInHouseTime / 60;
	m_iHouseSecond = (_int)tDesc.fInHouseTime % 60;

	m_iGhostMinute = (_int)tDesc.fAroundGhostTime / 60;
	m_iGhostSecond = (_int)tDesc.fAroundGhostTime % 60;

	m_iEventCnt = tDesc.iEventCnt;
	m_iInteractionCnt = tDesc.iInteractionCnt;

	lstrcpy(m_szGhostName, GAMEINSTANCE->Get_GhostName());

  	return S_OK;
}

void CLobby_GameResult::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CLobby_GameResult::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CLobby_GameResult::Render()
{
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_tchar	m_szTime[100];

	wsprintf(m_szTime, TEXT("%02d분 %02d초"), m_iTotalMinute,m_iTotalSecond);
	pGameInstance->Render_Font(TEXT("Font_Dream"), m_szTime, _float2(600.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	wsprintf(m_szTime, TEXT("%02d분 %02d초"), m_iHouseMinute, m_iHouseSecond);
	pGameInstance->Render_Font(TEXT("Font_Dream"), m_szTime, _float2(600.f, 200.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	wsprintf(m_szTime, TEXT("%02d분 %02d초"), m_iGhostMinute, m_iGhostSecond);
	pGameInstance->Render_Font(TEXT("Font_Dream"), m_szTime, _float2(600.f, 300.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	wsprintf(m_szTime, TEXT("%02d 회"), m_iEventCnt);
	pGameInstance->Render_Font(TEXT("Font_Dream"), m_szTime, _float2(600.f, 400.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	wsprintf(m_szTime, TEXT("%02d 회"), m_iInteractionCnt);
	pGameInstance->Render_Font(TEXT("Font_Dream"), m_szTime, _float2(600.f, 500.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;

}

void CLobby_GameResult::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
}

HRESULT CLobby_GameResult::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_GameResult"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLobby_GameResult::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

	_bool	bAlpha = false;

	if (FAILED(m_pShaderCom->Set_RawValue("bAlpha", &bAlpha, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;




	return S_OK;
}

HRESULT CLobby_GameResult::SetUp_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CIcon* pIcon;
	_matrix MainWorldMat = m_pTransformCom->Get_WorldMatrix();
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//뒤로
	_matrix IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 3.14f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.1f + IconWorld.r[1] * 1.71f;

	pIcon->Set_Transform(IconWorld);
	pIcon->Set_Texture(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_OutLine"));
	m_vecUIIcon.push_back(pIcon);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CLobby_GameResult * CLobby_GameResult::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby_GameResult*		pInstance = new CLobby_GameResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby_GameResult");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CLobby_GameResult::Clone(void * pArg)
{
	CLobby_GameResult*		pInstance = new CLobby_GameResult(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLobby_GameResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby_GameResult::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
