#pragma once

#include "Base.h"

/* �� �ִϸ��̼Ǹ��� ���� �Ҵ�Ǿ� �����ȴ�. */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);

	void Update_TransformationMatrices(_float fCurrentTime);
	void Reset_KeyFrame();

public:
	HRESULT SetUp_HierarchyNodePtr(class CModel* pModel);
private:
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrentKeyFrame = 0;



public:
	static CChannel* Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	CChannel* Clone(class CModel* pModel);

	virtual void Free() override;
};

END