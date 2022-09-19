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


	m_iAggressionLine = rand() % 25;
	m_iCalmLine = rand() % 11 + 60;

	return S_OK;
}

void CGhost_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTime -= fTimeDelta;
	m_fEventCoolTime += fTimeDelta;

	if (m_fTime >= 0.1f)
	{
		_float fValue = rand() % 101 * m_fAggressionWeight;
		fValue += rand() % 101 * m_fCalmWeight;
		fValue += rand() % 101 * (1.f - m_fAggressionWeight - m_fCalmWeight);

		if (fValue >= (_float)m_iCalmLine)
		{
			/*빡침 내려감*/
			if (0 < m_iAggression)
				--m_iAggression;
		}
		else if (fValue < (_float)m_iAggressionLine)
		{
			if (10 > m_iAggression)
				++m_iAggression;
		}
		m_fTime = 0.1f;
	}

	if (100.f < m_fEventCoolTime)
	{
		if (5 < m_iAggression && 10 > m_iAggression)
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


}

void CGhost_Status::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CGhost_Status::Render()
{

	return S_OK;
}

void CGhost_Status::Increase_BaseLine()
{
	m_iAggressionLine += 20;
	m_iCalmLine += 20;
}

void CGhost_Status::Decrease_BaseLine()
{
	m_iAggressionLine -= 20;
	m_iCalmLine -= 20;
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

}
