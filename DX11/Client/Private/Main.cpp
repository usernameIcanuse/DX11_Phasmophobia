#include "stdafx.h"
#include "..\Public\Main.h"
#include "GameInstance.h"
#include "UIIcon.h"
#include "Level_Loading.h"
#include "SoundMgr.h"

CMain::CMain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CMain::CMain(const CMain& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CMain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	m_fSizeX = 900.f;
	m_fSizeY = 600.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

  	return S_OK;
}

void CMain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (2 == m_iSelectedMenu)
	{
		LEVEL CurLevel = (LEVEL)GAMEINSTANCE->Get_Current_Level();
		if(LEVEL_GAMEPLAY == CurLevel)
			GAMEINSTANCE->Add_ReserveLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY, false, true), LEVEL_LOBBY);
		CSoundMgr::Get_Instance()->StopAll();
	}
}

void CMain::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI,this);
}

HRESULT CMain::Render()
{
	if (FAILED(SetUp_ShaderResource(nullptr, nullptr)))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMain::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4 ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	_bool	bAlpha = false;

	if (FAILED(m_pShaderCom->Set_RawValue("bAlpha", &bAlpha, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

void CMain::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for(auto& elem : m_vecUIIcon)
		elem->Set_Enable(_bEnable);
}

void CMain::Icon_Lock(_bool _bLock)
{
	for (auto& elem : m_vecUIIcon)
	{
		elem->Set_Lock(_bLock);
	}
}

HRESULT CMain::Setup_Component()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Main"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CMain::Setup_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUIIcon* pIcon = nullptr;
	//계속하기
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Main"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;

	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Resume"));
	pIcon->Set_IconPosition(420, 310, 300, 70);
	pIcon->Set_PassIndex(3);

	//떠나기

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Main"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;

	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exit"));
	pIcon->Set_IconPosition(420, 465, 250, 70);
	pIcon->Set_PassIndex(3);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMain * CMain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMain*		pInstance = new CMain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMain");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CMain::Clone(void * pArg)
{
	CMain*		pInstance = new CMain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
