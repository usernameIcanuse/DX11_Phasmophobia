#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� �����ϴ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}
	
	// D3DXCreateTextureFromFileEx :  �̹��� ���Ͽ� ����Ͽ� �ؽ�ó com��ü ������ִ� �Լ�
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// ����̽� �İ�ü
		pFilePath,		// ������ ���
		m_pTexInfo->tImgInfo.Width,		// ���� ���λ�����
		m_pTexInfo->tImgInfo.Height,	// ���� ���λ�����
		m_pTexInfo->tImgInfo.MipLevels,	// ���� �� ����
		0,								// �ؽ�ó ���� ���, 0�� ��� �Ϲ����� �ؽ�ó(D3DUSAGE_RENDERTARGET : ȭ�� ��¿� �ؽ�ó)
		m_pTexInfo->tImgInfo.Format,	// �ؽ�ó �ȼ� ����
		D3DPOOL_MANAGED,				// DX�ڿ����� � �޸𸮿� ������ ���ΰ�
		D3DX_DEFAULT,					// �̹��� ���͸� ���, Ȯ�� ��� �� ��� ä�� ���ΰ�	
		D3DX_DEFAULT,					// Mipfilter, �Ӹ��� �̿��� Ȯ�� ��� �� ��� ä�� ���ΰ�
		0,								// colorkey : ������ ����, 0�� �־��ָ� �÷�Ű ��ȿȭ
		nullptr,						// �̹��� ������ ����� ����ü �ּҰ�	, D3DXGetImageInfoFromFile�� ���ؼ� ä���༭ nullptr
		nullptr,						// �ȷ�Ʈ �̹��� ����, 8��Ʈ ������ argb�� ǥ���ϱ� ���� ����ü
		&(m_pTexInfo->pTexture))))
	{
		AfxMessageBox(L"Single Texture Load Failed");
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}


	/*D3DPOOL_DEFAULT = 0,	// ���� ������ �޸�(���� ī�� �޸�), ��ġ�� �սǵǾ��� ��� ������ �����, �ӵ��� ���� ����
	D3DPOOL_MANAGED = 1,	//  DX�ڿ��� �׷��� �޸� �����ϰ� ���ÿ� RAM�� ���, ���� ������
	D3DPOOL_SYSTEMMEM = 2,	// DX�ڿ��� RAM�� ����
	D3DPOOL_SCRATCH = 3,	// RAM�� ���������� DX ��ġ�� ������ �� ����*/

	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}

