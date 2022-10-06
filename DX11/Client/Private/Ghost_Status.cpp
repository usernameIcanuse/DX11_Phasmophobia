#include "stdafx.h"
#include "Ghost_Status.h"
#include "GameInstance.h"


CGhost_Status::CGhost_Status(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CGhost_Status::CGhost_Status(const CGhost_Status& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGhost_Status::Initialize_Prototype()
{
	if (__super::Initialize_Prototype())
		return E_FAIL;



	return S_OK;
}

HRESULT CGhost_Status::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	
	if (FAILED(__super::Initialize(&TransformDesc)))
		return E_FAIL;

	GAMEINSTANCE->Add_EventObject(CGame_Manager::EVENT_GHOST, this);

#ifdef _DEBUG
	if (__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderercom))
		return E_FAIL;
#endif
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 20);

	Add_Score(PLAYER_IN_HOUSE);
	
	/*m_fAttackCoolTime = */m_fEventCoolTime = dis(gen) + 20;


	return S_OK;
}

void CGhost_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif
	m_fTime += fTimeDelta;
	if (m_iCnt < 10 && 60.f < m_fTime)
	{
		Add_Score(TIME_ATTACK);
		m_fTime = 0.f;
		++m_iCnt;
	}
	m_iEMF = 1; 

	
	/*if(!m_bAttack)
		m_fAttackCoolTime -= fTimeDelta;*/

	if (m_iScore > 30)
	{
		/*5단계*/
		m_iEventWeight = 50;
		//m_iAttackWeight = 45;
	}
	else if (m_iScore > 24)
	{
		/*4단계*/
		m_iEventWeight = 40;
		//m_iAttackWeight = 35;
	}
	else if (m_iScore > 16)
	{
		/*3단계*/
		m_iEventWeight = 30;
		//m_iAttackWeight = 25;
	}
	else if (m_iScore > 8)
	{
		/*2단계*/
		m_iEventWeight = 15;
		//m_iAttackWeight = 15;
	}
	else
	{
		/*1단계*/
		m_iEventWeight = 5;
		//m_iAttackWeight = 5;
	}
	if (m_iScore > 4)
	{
		if (!m_bEvent)
			m_fEventCoolTime -= fTimeDelta;
		if (0.f > m_fEventCoolTime)
		{
			if (false == m_bEvent)
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 100);

				_int iValue = dis(gen);
				if (m_iEventWeight < iValue)
					wsprintf(m_szEventMessage, TEXT("Event"));
				else
				{
					wsprintf(m_szEventMessage, TEXT("Attack"));
					m_bAttack = true;
				}
				m_bEvent = true;
				m_bTerm = true;
			}
			else
			{
				m_iEMF = m_iScore / 6;
				if (m_iEMF > 4)
					m_iEMF = 4;

				if (m_bAttack && m_iScore > 24)
				{
					if (m_bEMFLevel5)
						m_iEMF = 5;
				}
				if (m_bTerm)
				{
					m_fTermBeforeEvent -= fTimeDelta;
					if (0.f > m_fTermBeforeEvent)
					{
						GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, m_szEventMessage);
						GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Event"));
						m_bTerm = false;
					}
				}
			}
		}
	}
}

void CGhost_Status::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	m_pRenderercom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
#endif
}

HRESULT CGhost_Status::Render()
{
#ifdef _DEBUG
	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szAgression, TEXT("공격성 : %d"), m_iScore);
		m_fTimeAcc = 0.f;
	}
	GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szAgression, _float2(0.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

#endif
	return S_OK;
}

void CGhost_Status::OnEventMessage(const _tchar* pMessage)
{
	if (0 == lstrcmp(TEXT("Normal_Operation"), pMessage))
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 100);

		m_bEvent = false;
		m_bAttack = false;
		m_fEventCoolTime = dis(gen) + 40 - m_iEventWeight;
		m_fTermBeforeEvent = dis(gen) % 3 + 2;
		
	}
	//else if (0 == lstrcmp(TEXT("Event"), pMessage))
	//{
	//	m_bEvent = true;
	//}
	//else if (0 == lstrcmp(TEXT("Attack"), pMessage))
	//{
	//	m_bEvent = true;
	//}
}


CGhost_Status* CGhost_Status::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGhost_Status* pInstance = new CGhost_Status(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGhost_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost_Status::Clone(void* pArg)
{
	CGhost_Status* pInstance = new CGhost_Status(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGhost_Status");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Status::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pRenderercom);
#endif

}
