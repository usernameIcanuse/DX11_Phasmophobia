#include "stdafx.h"
#include "..\Public\Lobby_WaitingRoom.h"
#include "GameInstance.h"
#include "UIIcon.h"


CLobby_WaitingRoom::CLobby_WaitingRoom(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CLobby_WaitingRoom::CLobby_WaitingRoom(const CLobby_WaitingRoom& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CLobby_WaitingRoom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLobby_WaitingRoom::Initialize(void * pArg)
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

  	return S_OK;
}

void CLobby_WaitingRoom::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CLobby_WaitingRoom::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CLobby_WaitingRoom::Render()
{


	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CLobby_WaitingRoom::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);
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

HRESULT CLobby_WaitingRoom::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;*/
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

HRESULT CLobby_WaitingRoom::SetUp_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUIIcon* pIcon;

	_matrix MainWorldMat = m_pTransformCom->Get_WorldMatrix();
	//의뢰 선택하기
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	_matrix IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 2.65f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 3.2f - IconWorld.r[0]*1.22f;

	pIcon->Set_Transform(IconWorld);
	pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Large_outline"));
	m_vecUIIcon.push_back(pIcon);

	////추가
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 1.6f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 1.85f + IconWorld.r[0] * 0.75f;

	pIcon->Set_Transform(IconWorld); pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	////구입
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 1.6f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 1.85f + IconWorld.r[0] * 1.85f;

	pIcon->Set_Transform(IconWorld);
	pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	//떠나기
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 1.8f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 3.2f + IconWorld.r[0] * 1.7f;

	pIcon->Set_Transform(IconWorld);
	pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	////준비
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 2.f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 3.2f - IconWorld.r[0] * 0.3f;

	pIcon->Set_Transform(IconWorld);
	pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

	////시작
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOBBY, TEXT("Layer_WaitingRoom"), TEXT("Prototype_GameObject_LobbyIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	IconWorld = XMMatrixIdentity();
	IconWorld.r[0] = XMVector3Normalize(MainWorldMat.r[0]) * 2.f;
	IconWorld.r[1] = XMVector3Normalize(MainWorldMat.r[1]) * 0.8f;
	IconWorld.r[2] = XMVector3Normalize(MainWorldMat.r[2]);
	IconWorld.r[3] = MainWorldMat.r[3] - IconWorld.r[2] * 0.01f - IconWorld.r[1] * 3.2f + IconWorld.r[0] * 0.6f;

	pIcon->Set_Transform(IconWorld); 
	pIcon->Set_Texture(TEXT("Prototype_Component_Texture_Small_outline"));
	m_vecUIIcon.push_back(pIcon);

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
