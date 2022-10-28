#include "stdafx.h"
#include "..\Public\UIIcon.h"

#include "GameInstance.h"

CUIIcon::CUIIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CIcon(pDevice, pContext)
{
}

CUIIcon::CUIIcon(const CUIIcon& rhs)
	: CIcon(rhs)
{
}

HRESULT CUIIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIIcon::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;


	
  	return S_OK;
}

void CUIIcon::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));

	RECT rect{ m_fX - (m_fSizeX * 0.5f), m_fY - (m_fSizeY * 0.5f),m_fX + (m_fSizeX * 0.5f), m_fY + (m_fSizeY * 0.5f) };
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	m_bSelected = false;
	m_bOnMouse = false;

	if (PtInRect(&rect, pt))
	{
		m_bOnMouse = true;
		if (CGameInstance::Get_Instance()->Is_KeyState(KEY::LBUTTON, KEY_STATE::TAP))
		{
			m_bSelected = true;
		}
	}
	
}

void CUIIcon::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUIIcon::Render()
{	
	if (FAILED(SetUp_ShaderResource(nullptr, nullptr)))
		return E_FAIL;

	if (m_iPassIndex == 4)
		int a = 10;
	m_pShaderCom->Begin(m_iPassIndex);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUIIcon::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
}


HRESULT CUIIcon::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("bAlpha", &m_bOnMouse, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bExcept", &m_bExcepted, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTexIndex)))
		return E_FAIL;


	return S_OK;
}

CUIIcon * CUIIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIIcon*		pInstance = new CUIIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIIcon");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CUIIcon::Clone(void * pArg)
{
	CUIIcon*		pInstance = new CUIIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIIcon::Free()
{
	__super::Free();

}
