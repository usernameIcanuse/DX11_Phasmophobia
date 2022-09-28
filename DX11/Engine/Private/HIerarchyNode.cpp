#include "..\Public\HIerarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::Initialize(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix)
{
	m_iDepth = iDepth;

	m_pParent = pParent;

	Safe_AddRef(m_pParent);

	strcpy_s(m_szName, pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_TransformMatrix = TransformMatrix;	

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr == m_pParent)
		m_CombinedTransformationMatrix = m_TransformationMatrix;

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
}

CHierarchyNode * CHierarchyNode::Create(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth, _float4x4 TransformMatrix)
{
	CHierarchyNode*		pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pAINode, pParent, iDepth, TransformMatrix)))
	{
		MSG_BOX("Failed to Created : CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode * CHierarchyNode::Clone()
{
	CHierarchyNode*		pInstance = new CHierarchyNode(*this);



	return nullptr;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}

