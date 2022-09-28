#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
	
	
	vector<class CChannel*>		m_Channels; /* ���� �ִԿ��� ���Ǵ� ���� ���¸� ������ ��ü�̴�. */

	
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_iNumChannels(rhs.m_iNumChannels)
	, m_fDuration(rhs.m_fDuration)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fTimeAcc(rhs.m_fTimeAcc)
	, m_isFinished(rhs.m_isFinished)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CAnimation::Initialize(aiAnimation * pAIAnimation, class CModel* pModel)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_iNumChannels = pAIAnimation->mNumChannels;

	/* ���� �ִϸ��̼��� ����ϴµ� �ɸ��� �ð�. */
	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	for (_uint i = 0; i < m_iNumChannels; ++i)	
	{
		aiNodeAnim*		pAIChannel = pAIAnimation->mChannels[i];

		CChannel*		pChannel = CChannel::Create(pAIChannel, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}	

	return S_OK;
}

void CAnimation::Update_TransformationMatrices(_float fTimeDelta)
{
	m_fTimeAcc += m_fTickPerSecond * fTimeDelta;
	
	if (m_fTimeAcc >= m_fDuration)
	{
		m_isFinished = true;
		m_fTimeAcc = 0.f;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished)
		{
			m_Channels[i]->Reset_KeyFrame();
		}


		m_Channels[i]->Update_TransformationMatrices(m_fTimeAcc);
	}





}

HRESULT CAnimation::Clone_Channel(CAnimation * pPrototype, CModel * pModel)
{
	for (auto& pPrototypeChannel : pPrototype->m_Channels)
	{
		m_Channels.push_back(pPrototypeChannel->Clone(pModel));
	}

	return S_OK;
}

CAnimation * CAnimation::Create(aiAnimation * pAIAnimation, CModel* pModel)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Clone(CModel * pModel)
{
	CAnimation*		pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Clone_Channel(this, pModel)))
		return nullptr;

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}
