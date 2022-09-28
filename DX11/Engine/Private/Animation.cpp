#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
	
	
	vector<class CChannel*>		m_Channels; /* 현재 애님에서 사용되는 뼈의 상태를 저장한 객체이다. */

	
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

	/* 현재 애니메이션을 재상하는데 걸리는 시간. */
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
