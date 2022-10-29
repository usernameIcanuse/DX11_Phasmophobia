#include "stdafx.h"
#include "..\Public\Evidence.h"
#include "GameInstance.h"
#include "UIIcon.h"

CEvidence::CEvidence(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIBackground(pDevice, pContext)
{
}

CEvidence::CEvidence(const CEvidence& rhs)
	: CUIBackground(rhs)
{
}

HRESULT CEvidence::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEvidence::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Setup_Component()))
		return E_FAIL;

	//if (FAILED(Setup_Icon()))
	//	return E_FAIL;

	m_fSizeX = 900.f;
	m_fSizeY = 600.f;
	m_fX = g_iWinCX >> 1;
	m_fY = g_iWinCY >> 1;

	// XMMatrixPerspectiveFovLH()
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));


	Set_GhostEvidence();

  	return S_OK;
}

void CEvidence::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_int iIndex = 0;

	for (auto& elem : m_IconEvidence)
	{
		if (elem->Selected())
		{
			m_arrEvidence[iIndex] += 1;
			if (m_arrEvidence[iIndex] == 2)
				m_arrEvidence[iIndex] = -1;

			elem->Set_MultiTex_Index(m_arrEvidence[iIndex] + 1);
		}

		++iIndex;
	}


	for (_int i = 0; i < 24; ++i)
	{
		_int iEvidenceCnt = 0;
		_int iGhostEvidence = 0;

		_int iExceptedCnt = 0;
		_int iGhostExceptedCnt = 0;

		_bool bFlag = false;

		for (_int j = 0; j < 7; ++j)
		{
			_int iEvidence = m_arrEvidence[j];
			if (-1 == iEvidence)
				continue;

			if (0 == iEvidence)
			{
				++iEvidenceCnt;
			}
			else if (1 == iEvidence)
			{
				++iExceptedCnt;
			}

			bFlag = true;

			for (auto& elem : m_vecGhostEvidence[i])
			{
				_int iEvidenceIndex = elem;
				if (0 == iEvidence)
				{

					if (iEvidenceIndex == j)
						++iGhostEvidence;
				}
				else if (1 == iEvidence)
				{

					if (iEvidenceIndex == j)
						++iGhostExceptedCnt;
				}
			}

		}

		if (false == bFlag)
		{
			static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(false);
		}
		else
		{
			if (0 < iGhostExceptedCnt)
				static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(true);
			else
			{
				if (0 != iEvidenceCnt && iEvidenceCnt == iGhostEvidence)
				{
					static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(false);
				}
				else if(0== iEvidenceCnt)
				{
					static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(false);
				}
				else
				{
					static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(true);
				}
			}
		}
	/*	if(0!=iExceptedCnt && 0 < iGhostExceptedCnt)
			static_cast<CUIIcon*>(m_vecUIIcon[i])->Is_Excepted(true);*/
	}
}

void CEvidence::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEvidence::Render()
{
	if (FAILED(SetUp_ShaderResource(nullptr, nullptr)))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CEvidence::Set_Enable(_bool _bEnable)
{
	__super::Set_Enable(_bEnable);

	for (auto& elem : m_IconEvidence)
		elem->Set_Enable(_bEnable);

	for(auto& elem : m_vecUIIcon)
		elem->Set_Enable(_bEnable);
}

void CEvidence::Icon_Lock(_bool _bLock)
{
	for (auto& elem : m_vecUIIcon)
	{
		elem->Set_Lock(_bLock);
	}
}

HRESULT CEvidence::Setup_Component()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_Evidence"), TEXT("Com_Texture "), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CEvidence::Setup_Icon()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	/*Evidence Icon*/

	CUIIcon* pIcon = nullptr;
#pragma region 증거
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//emf
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(755, 165, 180, 35);
	pIcon->Set_PassIndex(3);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//도트
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(935, 165, 180, 35);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//손자국
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(755, 195, 180, 35);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//오브
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(935, 195, 180, 35);
	pIcon->Set_PassIndex(4);
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//라이팅
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(755, 225, 180, 35);
	pIcon->Set_PassIndex(4);
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//주파수
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(935, 225, 180, 35);
	pIcon->Set_PassIndex(4);
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//서늘함
	m_IconEvidence.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_SelectEvidence_Texture"));
	pIcon->Set_IconPosition(755, 260, 180, 35);
	pIcon->Set_PassIndex(4);

#pragma endregion 증거

	/*GhostName_Icon*/

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//스피릿
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_spirit"));
	pIcon->Set_IconPosition(720, 370, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//레이스
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_wraith"));
	pIcon->Set_IconPosition(845, 370, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//팬텀
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_phantom"));
	pIcon->Set_IconPosition(970, 370, 110, 30);
	pIcon->Set_PassIndex(4);


	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//폴터가이스트
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_poltergeist"));
	pIcon->Set_IconPosition(720, 400, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//밴시
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_banshee"));
	pIcon->Set_IconPosition(845, 400, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//진
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_jinn"));
	pIcon->Set_IconPosition(970, 400, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//메어
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_mare"));
	pIcon->Set_IconPosition(720, 430, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//레버넌트
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_revenant"));
	pIcon->Set_IconPosition(845, 430, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//셰이드
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_shade"));
	pIcon->Set_IconPosition(970, 430, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//데몬
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_demon"));
	pIcon->Set_IconPosition(720, 460, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//유레이
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_yurei"));
	pIcon->Set_IconPosition(845, 460, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//오니
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_oni"));
	pIcon->Set_IconPosition(970, 460, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//요괴
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_yokai"));
	pIcon->Set_IconPosition(720, 490, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//한투
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_hantu"));
	pIcon->Set_IconPosition(845, 490, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//고료
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_goryo"));
	pIcon->Set_IconPosition(970, 490, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//마일링
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_myling"));
	pIcon->Set_IconPosition(720, 520, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//원령
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_onryo"));
	pIcon->Set_IconPosition(845, 520, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//트윈스
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_twins"));
	pIcon->Set_IconPosition(970, 520, 110, 30);
	pIcon->Set_PassIndex(4);

	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//라이주
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_raiju"));
	pIcon->Set_IconPosition(720, 550, 110, 30);
	pIcon->Set_PassIndex(4); 
	
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//오바케
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_obake"));
	pIcon->Set_IconPosition(845, 550, 110, 30);
	pIcon->Set_PassIndex(4); 
	
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//미믹
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_mimic"));
	pIcon->Set_IconPosition(970, 550, 110, 30);
	pIcon->Set_PassIndex(4); 
	
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//모로이
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_moroi"));
	pIcon->Set_IconPosition(720, 580, 110, 30);
	pIcon->Set_PassIndex(4); 
	
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//데오겐
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_deogen"));
	pIcon->Set_IconPosition(845, 580, 110, 30);
	pIcon->Set_PassIndex(4); 
	
	if (FAILED(pGameInstance->Add_GameObject(pGameInstance->Get_Next_Level(), TEXT("Layer_UIIcon_Evidence"), TEXT("Prototype_GameObject_UIIcon"), (CGameObject**)&pIcon)))
		return E_FAIL;
	//타예
	m_vecUIIcon.push_back(pIcon);
	pIcon->Set_Texture(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Journal_GhostName_thaye"));
	pIcon->Set_IconPosition(970, 580, 110, 30);
	pIcon->Set_PassIndex(4);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEvidence::SetUp_ShaderResource(_float4x4* pViewMatrix, _float4x4* pProjMatrix)
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

void CEvidence::Set_GhostEvidence()
{
	m_vecGhostEvidence[0].push_back(0);
	m_vecGhostEvidence[0].push_back(4);
	m_vecGhostEvidence[0].push_back(5);
	
	
	m_vecGhostEvidence[1].push_back(0);
	m_vecGhostEvidence[1].push_back(1);
	m_vecGhostEvidence[1].push_back(5);
	
	
	m_vecGhostEvidence[2].push_back(1);
	m_vecGhostEvidence[2].push_back(2);
	m_vecGhostEvidence[2].push_back(5);
	
	m_vecGhostEvidence[3].push_back(2);
	m_vecGhostEvidence[3].push_back(4);
	m_vecGhostEvidence[3].push_back(5);
	
	
	m_vecGhostEvidence[4].push_back(1);
	m_vecGhostEvidence[4].push_back(2);
	m_vecGhostEvidence[4].push_back(3);
	
	
	m_vecGhostEvidence[5].push_back(0);
	m_vecGhostEvidence[5].push_back(2);
	m_vecGhostEvidence[5].push_back(6);
	
	
	m_vecGhostEvidence[6].push_back(3);
	m_vecGhostEvidence[6].push_back(4);
	m_vecGhostEvidence[6].push_back(5);
	
	
	m_vecGhostEvidence[7].push_back(3);
	m_vecGhostEvidence[7].push_back(4);
	m_vecGhostEvidence[7].push_back(6);
	
	m_vecGhostEvidence[8].push_back(0);
	m_vecGhostEvidence[8].push_back(4);
	m_vecGhostEvidence[8].push_back(6);
	
	
	m_vecGhostEvidence[9].push_back(2);
	m_vecGhostEvidence[9].push_back(4);
	m_vecGhostEvidence[9].push_back(6);
	
	
	m_vecGhostEvidence[10].push_back(1);
	m_vecGhostEvidence[10].push_back(3);
	m_vecGhostEvidence[10].push_back(6);
	
	m_vecGhostEvidence[11].push_back(0);
	m_vecGhostEvidence[11].push_back(1);
	m_vecGhostEvidence[11].push_back(6);
	
	
	m_vecGhostEvidence[12].push_back(1);
	m_vecGhostEvidence[12].push_back(3);
	m_vecGhostEvidence[12].push_back(5);
	
	
	m_vecGhostEvidence[13].push_back(2);
	m_vecGhostEvidence[13].push_back(3);
	m_vecGhostEvidence[13].push_back(6);
	
	
	m_vecGhostEvidence[14].push_back(0);
	m_vecGhostEvidence[14].push_back(1);
	m_vecGhostEvidence[14].push_back(2);
	
	m_vecGhostEvidence[15].push_back(0);
	m_vecGhostEvidence[15].push_back(2);
	m_vecGhostEvidence[15].push_back(4);
	
	
	m_vecGhostEvidence[16].push_back(3);
	m_vecGhostEvidence[16].push_back(5);
	m_vecGhostEvidence[16].push_back(6);
	
	
	m_vecGhostEvidence[17].push_back(0);
	m_vecGhostEvidence[17].push_back(5);
	m_vecGhostEvidence[17].push_back(6);
	
	
	m_vecGhostEvidence[18].push_back(0);
	m_vecGhostEvidence[18].push_back(1);
	m_vecGhostEvidence[18].push_back(3);
	
	m_vecGhostEvidence[19].push_back(0);
	m_vecGhostEvidence[19].push_back(2);
	m_vecGhostEvidence[19].push_back(3);
	
	
	m_vecGhostEvidence[20].push_back(2);
	m_vecGhostEvidence[20].push_back(5);
	m_vecGhostEvidence[20].push_back(6);
	
	m_vecGhostEvidence[21].push_back(4);
	m_vecGhostEvidence[21].push_back(5);
	m_vecGhostEvidence[21].push_back(6);
	
	
	m_vecGhostEvidence[22].push_back(1);
	m_vecGhostEvidence[22].push_back(4);
	m_vecGhostEvidence[22].push_back(5);
	
	m_vecGhostEvidence[23].push_back(1);
	m_vecGhostEvidence[23].push_back(3);
	m_vecGhostEvidence[23].push_back(4);
	
}

CEvidence * CEvidence::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEvidence*		pInstance = new CEvidence(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEvidence");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CEvidence::Clone(void * pArg)
{
	CEvidence*		pInstance = new CEvidence(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEvidence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvidence::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

}
