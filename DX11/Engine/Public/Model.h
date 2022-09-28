#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODELTYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshContainers() const {
		return m_iNumMeshContainers;
	}

	void Set_CurrentAnimation(_uint iAnimIndex) {
		m_iCurrentAnimationIndex = iAnimIndex;
	}

	class CHierarchyNode* Find_HierarcyNode(const char* pBoneName);
	

public:
	HRESULT Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	HRESULT Initialize(void* pArg, CModel* pPrototype);
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Render(_uint iMeshContainerIndex, class CShader* pShader, _uint iPassIndex = 0, const char* pConstantBoneName = nullptr);

public:
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, _uint iMeshContainerIndex, aiTextureType eType);

private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	MODELTYPE					m_eModelType = TYPE_END;
	_float4x4					m_TransformMatrix;

private:
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	_uint			m_iNumMeshContainers = 0;

private:
	vector<MODEL_MATERIAL>					m_Materials;
	typedef vector<MODEL_MATERIAL>			MATERIALS;
	_uint			m_iNumMaterials = 0;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;


private:
	HRESULT Create_MeshContainers();
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Create_Animations();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END