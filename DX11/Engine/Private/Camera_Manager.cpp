#include "..\Public\Camera_Manager.h"
#include "GameObject.h"
#include "Camera.h"

IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{
}


void CCamera_Manager::Tick(_float fTimeDelta)
{
	if (nullptr != m_pCurrCamera)
	{
		if (FAILED(m_pCurrCamera->Bind_PipeLine()))
			return;
	}
}

HRESULT CCamera_Manager::Add_CameraObject(const _tchar* pCameraTag, CCamera* pObject)
{
	CCamera* pCamera= Find_Camera(pCameraTag);
	if (nullptr == pCamera)
	{
		m_Cameras.emplace(pCameraTag, pObject);
		Safe_AddRef(pObject);
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Manager::Change_Camera(const _tchar* pCameraTag)
{
	CCamera* pCamera = Find_Camera(pCameraTag);
	if (nullptr == pCamera)
		return E_FAIL;

	m_pCurrCamera = pCamera;
	m_pCurrCameraTag = pCameraTag;

	if (FAILED(m_pCurrCamera->Bind_PipeLine()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Manager::Current_Camera(const _tchar* pCameraTag)
{
	if (0 == lstrcmp(pCameraTag, m_pCurrCameraTag))
		return S_OK;

	return E_FAIL;
}

void CCamera_Manager::Clear_Cameras()
{
	for (auto& elem : m_Cameras)
	{
		Safe_Release(elem.second);
	}
	m_Cameras.clear();
}

CCamera* CCamera_Manager::Find_Camera(const _tchar* pCameraTag)
{
	auto	iter = find_if(m_Cameras.begin(), m_Cameras.end(), CTag_Finder(pCameraTag));

	if (iter == m_Cameras.end())
		return nullptr;

	return iter->second;

}

void CCamera_Manager::Free()
{
	for (auto& elem : m_Cameras)
	{
		Safe_Release(elem.second);
	}
	m_Cameras.clear();

}
