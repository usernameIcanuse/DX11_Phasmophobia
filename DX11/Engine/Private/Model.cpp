#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_MeshContainers(rhs.m_MeshContainers)	
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
{
	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_AddRef(pTexture);
	}

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

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

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}



HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;	
		
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

	if (nullptr == m_Materials[iMaterialIndex].pTextures[eType])
	{
		m_Materials[iMaterialIndex].pTextures[eType] = CTexture::Create(m_pDevice, m_pContext,
			TEXT("..\\Bin\\Resources\\Textures\\Default_Texture.png"));

	}



	return m_Materials[iMaterialIndex].pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);	
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_TransformMatrix));
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
			{
				continue;
			}

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			if(0==strcmp(szExt, ".tga"))
				strcat_s(szFullPath, ".png");
			else
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

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

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
