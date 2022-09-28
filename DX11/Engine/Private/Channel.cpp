#include "..\Public\Channel.h"
#include "HIerarchyNode.h"
#include "Model.h"

CChannel::CChannel()
{
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrentKeyFrame = 0;
}

CChannel::CChannel(const CChannel& rhs)
	: m_iNumKeyframes(rhs.m_iNumKeyframes)
	, m_KeyFrames(rhs.m_KeyFrames)
	, m_iCurrentKeyFrame(rhs.m_iCurrentKeyFrame)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	Safe_AddRef(m_pHierarchyNode);

	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{


		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = (_float)pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;

	if (fCurrentTime > m_KeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fCurrentTime > m_KeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrame].fTime)
			/ (m_KeyFrames[m_iCurrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}



	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Reset_KeyFrame()
{
	m_iCurrentKeyFrame = 0;
}

HRESULT CChannel::SetUp_HierarchyNodePtr(CModel * pModel)
{
	m_pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	Safe_AddRef(m_pHierarchyNode);

	return S_OK;
}

CChannel * CChannel::Create(aiNodeAnim * pAIChannel, CModel* pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel * CChannel::Clone(CModel* pModel)
{
	CChannel*			pInstance = new CChannel(*this);

	if (FAILED(pInstance->SetUp_HierarchyNodePtr(pModel)))
		return nullptr;

	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pHierarchyNode);
}
