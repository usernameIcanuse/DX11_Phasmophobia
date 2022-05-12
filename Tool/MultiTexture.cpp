#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto	iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), [&](auto& Pair) 
	{
		if (pStateKey == Pair.first)
			return true;

		return false;
	});
	
	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, 
									const TCHAR* pStateKey /*= L""*/, 
									const int& iCount /*= 0*/)
{
	// 520����Ʈ, ����� ���̴� ���� 520����Ʈ�� �Ѿ �� �ȴ�.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			AfxMessageBox(pFilePath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		// D3DXCreateTextureFromFileEx :  �̹��� ���Ͽ� ����Ͽ� �ؽ�ó com��ü ������ִ� �Լ�
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// ����̽� �İ�ü
			szFullPath,		// ������ ���
			pTexInfo->tImgInfo.Width,		// ���� ���λ�����
			pTexInfo->tImgInfo.Height,	// ���� ���λ�����
			pTexInfo->tImgInfo.MipLevels,	// ���� �� ����
			0,								// �ؽ�ó ���� ���, 0�� ��� �Ϲ����� �ؽ�ó(D3DUSAGE_RENDERTARGET : ȭ�� ��¿� �ؽ�ó)
			pTexInfo->tImgInfo.Format,	// �ؽ�ó �ȼ� ����
			D3DPOOL_MANAGED,				// DX�ڿ����� � �޸𸮿� ������ ���ΰ�
			D3DX_DEFAULT,					// �̹��� ���͸� ���, Ȯ�� ��� �� ��� ä�� ���ΰ�	
			D3DX_DEFAULT,					// Mipfilter, �Ӹ��� �̿��� Ȯ�� ��� �� ��� ä�� ���ΰ�
			0,								// colorkey : ������ ����, 0�� �־��ָ� �÷�Ű ��ȿȭ
			nullptr,						// �̹��� ������ ����� ����ü �ּҰ�	, D3DXGetImageInfoFromFile�� ���ؼ� ä���༭ nullptr
			nullptr,						// �ȷ�Ʈ �̹��� ����, 8��Ʈ ������ argb�� ǥ���ϱ� ���� ����ü
			&(pTexInfo->pTexture))))
		{
			AfxMessageBox(L"Multi Texture Load Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& Pair)
	{
		for_each(Pair.second.begin(), Pair.second.end(), Safe_Delete<TEXINFO*>);
		Pair.second.clear();
	});
	m_mapMultiTex.clear();
}
