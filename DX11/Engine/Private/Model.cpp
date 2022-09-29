#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Texture.h"
#include "Shader.h"


CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)	
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)	
	, m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex)
	, m_iNumAnimations(rhs.m_iNumAnimations)	
	, m_TransformMatrix(rhs.m_TransformMatrix)
{
	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_AddRef(pTexture);
	}
}

CHierarchyNode * CModel::Find_HierarcyNode(const char * pBoneName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode) 
	{
		return !strcmp(pBoneName, pNode->Get_Name());
	});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;

	
}

HRESULT CModel::Initialize_Prototype(MODELTYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix)
{
	m_eModelType = eType;

	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);	

	if (TYPE_NONANIM == eType)
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace );
	else
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Create_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) 
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	if (FAILED(Create_Animations()))
		return E_FAIL;

	return S_OK;
}



HRESULT CModel::Initialize(void * pArg, CModel* pPrototype)
{
	if (FAILED(Create_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	for (auto& pPrototypeMeshContainer : pPrototype->m_MeshContainers)
	{
		CMeshContainer*		pMeshContainer = (CMeshContainer*)pPrototypeMeshContainer->Clone();
		if (nullptr == pMeshContainer)
			return E_FAIL;
		pMeshContainer->SetUp_BonesPtr(this);

		m_MeshContainers.push_back(pMeshContainer);
	}

	for (auto& pPrototypeAnimation : pPrototype->m_Animations)
	{
		CAnimation*		pAnimation = pPrototypeAnimation->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}




	return S_OK;
}

HRESULT CModel::Play_Animation(_float fTimeDelta)
{
	if (m_iCurrentAnimationIndex >= m_iNumAnimations)
		return E_FAIL;

	m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(fTimeDelta);

	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Update_CombinedTransformationMatrix();

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex, const char* pConstantBoneName)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;	

	/* 그리고자하는 메시컨테이너에 영향을 주는 뼈들의 행렬을 담아준다. */
	_float4x4			BoneMatrices[256];

	m_MeshContainers[iMeshContainerIndex]->SetUp_BoneMatices(BoneMatrices, XMLoadFloat4x4(&m_TransformMatrix));

	if (0 != m_iNumAnimations)
		pShader->Set_RawValue(pConstantBoneName, BoneMatrices, sizeof(_float4x4) * 256);	
		
	pShader->Begin(iPassIndex);

	m_MeshContainers[iMeshContainerIndex]->Render();	

	return S_OK;
}

HRESULT CModel::Bind_SRV(CShader * pShader, const char * pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	/*if (nullptr == m_Materials[iMaterialIndex].pTextures[eType])
	{
		m_Materials[iMaterialIndex].pTextures[eType] = CTexture::Create(m_pDevice, m_pContext,
			TEXT("..\\Bin\\Resources\\Textures\\Default_Texture.png"));

	}*/

	return m_Materials[iMaterialIndex].pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);	
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_TransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char			szFullPath[MAX_PATH] = "";

			aiString		strPath;
						
			if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);		

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFilePath, MAX_PATH);

			Material.pTextures[j] = CTexture::Create(m_pDevice, m_pContext, szTextureFilePath);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;
		}

		m_Materials.push_back(Material);		
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pAINode, pParent, iDepth, m_TransformMatrix);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
		Create_HierarchyNodes(pAINode->mChildren[i], pHierarchyNode, iDepth + 1);

	return S_OK;
}

HRESULT CModel::Create_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation*		pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);		
	}	

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, MODELTYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix TransformMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName, TransformMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg, this)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_Release(pTexture);
	}
	m_Materials.clear();


	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	m_MeshContainers.clear();

	if(false == m_isCloned)
		m_Importer.FreeScene();

}
