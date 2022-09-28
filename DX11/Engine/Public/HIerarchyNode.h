#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;
public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	_matrix Get_TransformMatrix() const {
		return XMLoadFloat4x4(&m_TransformMatrix);
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix);
	void Update_CombinedTransformationMatrix();
private:
	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";

	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;
	_float4x4		m_TransformMatrix;

	CHierarchyNode*	m_pParent = nullptr;

	
public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix);
	CHierarchyNode* Clone();
	virtual void Free() override;

};

END