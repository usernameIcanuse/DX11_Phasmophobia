#include "stdafx.h"
#include "..\Public\Lobby_AddItems.h"
#include "GameInstance.h"
#include "UIIcon.h"


CLobby_AddItems::CLobby_AddItems(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CLobby_AddItems::CLobby_AddItems(const CLobby_AddItems& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CLobby_AddItems::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_AddItems::Initialize(void * pArg)
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

void CLobby_AddItems::Tick(_float fTimeDelta)
{


	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	__super::Tick(fTimeDelta);
}

void CLobby_AddItems::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CLobby_AddItems::Render()
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

void CLobby_AddItems::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
}

HRESULT CLobby_AddItems::SetUp_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_WaitingRoom"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLobby_AddItems::SetUp_ShaderResource()
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

	_bool	bAlpha = false;

	if (FAILED(m_pShaderCom->Set_RawValue("bAlpha", &bAlpha, sizeof(_bool))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLobby_AddItems::SetUp_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CIcon* pIcon;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"),(CGameObject**)& pIcon)))
		return E_FAIL;
	//의뢰 선택하기
	pIcon->Set_IconPosition(310.f, 560.f, 260.f, 70.f);
	pIcon->Set_Texture(LEVEL_LOBBY,TEXT("Prototype_Component_Texture_Large_outline"));
	m_vecUIIcon.push_back(pIcon);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//추가
	pIcon->Set_IconPosition(760.f, 475.f, 150.f, 70.f);
	pIcon->Set_Texture(LEVEL_LOBBY,TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//구입
	pIcon->Set_IconPosition(940.f, 475.f, 150.f, 70.f);
	pIcon->Set_Texture(LEVEL_LOBBY,TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//떠나기
	pIcon->Set_IconPosition(955.f, 555.f, 170.f, 60.f);
	pIcon->Set_Texture(LEVEL_LOBBY,TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CLobby_AddItems * CLobby_AddItems::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby_AddItems*		pInstance = new CLobby_AddItems(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby_AddItems");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CLobby_AddItems::Clone(void * pArg)
{
	CLobby_AddItems*		pInstance = new CLobby_AddItems(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLobby_AddItems");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby_AddItems::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
