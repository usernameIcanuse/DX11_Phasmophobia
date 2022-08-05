#include "stdafx.h"
#include "..\Public\Lobby_WaitingRoom.h"
#include "GameInstance.h"
#include "UIWaitingRoom.h"
#include "UIWaitingRoomSmall.h"//Create UIBase Class


CLobby_WaitingRoom::CLobby_WaitingRoom(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CLobby_WaitingRoom::CLobby_WaitingRoom(const CLobby_WaitingRoom& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLobby_WaitingRoom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_WaitingRoom::Initialize(void * pArg)
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

void CLobby_WaitingRoom::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	m_iSelectedMenu = 0;
	if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
	{
	}

	for (int i = 1; i < 4; ++i)
	{
		if (static_cast<CUIWaitingRoomSmall*>(m_pUIIcon[i])->Selected())
			m_iSelectedMenu = i + 1;
	}
}

void CLobby_WaitingRoom::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CLobby_WaitingRoom::Render()
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

void CLobby_WaitingRoom::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for (int i = 0; i < 4; ++i)
		m_pUIIcon[i]->Set_Enable(_bEnable);

}

HRESULT CLobby_WaitingRoom::SetUp_Components()
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

HRESULT CLobby_WaitingRoom::SetUp_ShaderResource()
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

HRESULT CLobby_WaitingRoom::SetUp_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_UIWaitingRoom"),&m_pUIIcon[0])))
		return E_FAIL;
	//의뢰 선택하기
	static_cast<CUIWaitingRoom*>(m_pUIIcon[0])->Set_IconPosition(310.f, 560.f, 260.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_UIWaitingRoomSmall"), &m_pUIIcon[1])))
		return E_FAIL;
	//추가
	static_cast<CUIWaitingRoom*>(m_pUIIcon[1])->Set_IconPosition(760.f, 475.f, 150.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_UIWaitingRoomSmall"), &m_pUIIcon[2])))
		return E_FAIL;
	//구입
	static_cast<CUIWaitingRoom*>(m_pUIIcon[2])->Set_IconPosition(940.f, 475.f, 150.f, 70.f);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_UIWaitingRoomSmall"), &m_pUIIcon[3])))
		return E_FAIL;
	//떠나기
	static_cast<CUIWaitingRoom*>(m_pUIIcon[3])->Set_IconPosition(955.f, 555.f, 170.f, 60.f);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CLobby_WaitingRoom * CLobby_WaitingRoom::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLobby_WaitingRoom*		pInstance = new CLobby_WaitingRoom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLobby_WaitingRoom");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CLobby_WaitingRoom::Clone(void * pArg)
{
	CLobby_WaitingRoom*		pInstance = new CLobby_WaitingRoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLobby_WaitingRoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLobby_WaitingRoom::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
