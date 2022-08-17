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


public:
	HRESULT Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	HRESULT Initialize(void* pArg) override;
	HRESULT Render(_uint iMeshContainerIndex);

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
	HRESULT Create_MeshContainers();
	HRESULT Create_Materials(const char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END