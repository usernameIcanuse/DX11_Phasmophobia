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

#ifdef _DEBUG
	if (__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderercom))
		return E_FAIL;
#endif
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 10);

	m_iCalmLine = dis(gen) + 15;
	m_iAggressionLine = dis(gen) + 70;

	m_iAggressionWeight = 5.f;//집에들어갈 때 5를 더해주는 걸로 지금은 미리 5를 더해주고 감

	m_fEventWeight = 0.6f;
	m_fAttackWeight = 0.4f;

	return S_OK;
}

void CGhost_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTimeAcc += fTimeDelta;

	m_fTime += fTimeDelta;
	m_fEventCoolTime += fTimeDelta;

	if (m_fTime >= 1.f)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, m_iAggressionLine+ (_int)m_iAggressionWeight);

		_float fValue = dis(gen);
		fValue *= (1.f + m_iAggressionWeight * 0.01f);
		
		if (fValue >= (_float)m_iAggressionLine)
		{
			if (10 > m_iAggression)
				++m_iAggression;
			
		}
		else if (fValue <= (_float)m_iCalmLine - m_iAggressionWeight)
		{
			if (0 < m_iAggression)
				--m_iAggression;
		}
		m_fTime = 0.f;
	}

	if (100.f < m_fEventCoolTime)
	{
		if (4 < m_iAggression && 10 > m_iAggression)
		{

			_float fRandomValue = rand() % 101 * m_fEventWeight;
			fRandomValue += rand() % 101 * (1.f - m_fEventWeight);
			if (20.f > fRandomValue)
			{
				m_fEventTime -= fTimeDelta;
				/*event OnMessage호출*/
				GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Event"));
				GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Event"));
				if (m_fEventTime < 0.f)
				{
					m_fEventTime = 10.f;
					m_fEventCoolTime = 0.f;
				}
			}


		}
		else if (10 == m_iAggression)
		{

			_float fRandomValue = rand() % 101 * m_fEventWeight;
			fRandomValue += rand() % 101 * (1.f - m_fEventWeight);
			if (20.f > fRandomValue)
			{
				m_fEventTime -= fTimeDelta;
				/*Attack*/
				GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_GHOST, TEXT("Attack"));
				GAMEINSTANCE->Broadcast_Message(CGame_Manager::EVENT_ITEM, TEXT("Event"));

				if (m_fAttackTime < 0.f)
				{
					m_fAttackTime = 10.f;
					m_fEventCoolTime = 0.f;
				}
			}

		}
	}

	m_iEMF = m_iAggression / 2+1;
	/*만약 5가 됐는데 emf 5단계 조건 있으면 그냥 대입*/

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
		wsprintf(m_szAgression, TEXT("공격성 : %d"), m_iAggression);
		m_fTimeAcc = 0.f;
	}
	GAMEINSTANCE->Render_Font(TEXT("Font_Dream"), m_szAgression, _float2(0.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

#endif
	return S_OK;
}

void CGhost_Status::Increase_BaseLine(WEIGHT eWeightType)
{
	m_iAggressionWeight += eWeightType;
}

void CGhost_Status::Decrease_BaseLine(WEIGHT eWeightType)
{
	m_iAggressionWeight -= eWeightType;
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
