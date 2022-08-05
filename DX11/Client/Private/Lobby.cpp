#include "stdafx.h"
#include "..\Public\Lobby.h"
#include "GameInstance.h"
#include "UIIcon.h"

CLobby::CLobby(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CLobby::CLobby(const CLobby& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLobby::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_Icon()))
		return E_FAIL;


	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

  	return S_OK;
}

void CLobby::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
	
	m_iSelectedMenu = 0;
	
	for (int i = 0; i < 4; ++i)
	{
		if (static_cast<CUIIcon*>(m_pUIIcon[i])->Selected())
			m_iSelectedMenu = i + 1;
	}
}

void CLobby::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CLobby::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CLobby::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for (int i = 0; i < 4; ++i)
		m_pUIIcon[i]->Set_Enable(_bEnable);

}



HRESULT CLobby::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLobby::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;*/
	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLobby::SetUp_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_LobbyIcon"),&m_pUIIcon[0])))
		return E_FAIL;
	//싱글 플레이
	static_cast<CUIIcon*>(m_pUIIcon[0])->Set_IconPosition(g_iWinCX >> 1, (g_iWinCY >> 1) - 110.f, 325.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_LobbyIcon"), &m_pUIIcon[1])))
		return E_FAIL;
	//장비 상점
	static_cast<CUIIcon*>(m_pUIIcon[1])->Set_IconPosition(g_iWinCX >> 1, (g_iWinCY >> 1) +35.f, 325.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_LobbyIcon"), &m_pUIIcon[2])))
		return E_FAIL;
	//옵션
	static_cast<CUIIcon*>(m_pUIIcon[2])->Set_IconPosition(g_iWinCX >> 1, (g_iWinCY >> 1) + 110.f, 325.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_Lobby"), TEXT("Prototype_GameObject_LobbyIcon"), &m_pUIIcon[3])))
		return E_FAIL;
	//게임종료
	static_cast<CUIIcon*>(m_pUIIcon[3])->Set_IconPosition(g_iWinCX >> 1, (g_iWinCY >> 1) + 180.f, 325.f, 70.f);



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLobby * CLobby::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby*		pInstance = new CLobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CLobby::Clone(void * pArg)
{
	CLobby*		pInstance = new CLobby(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
