#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

#include "Base.h"

BEGIN(Engine)


class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	void Tick(_float fTimeDelta);
	HRESULT Add_CameraObject(const _tchar* pCameraTag, class CCamera* pObject);
	HRESULT Change_Camera(const _tchar* pCameraTag);
	HRESULT Current_Camera(const _tchar* pCameraTag);
public:
	void Clear_Cameras();

private:
	CCamera* Find_Camera(const _tchar* pCameraTag);

private:
	map<const _tchar*, class CCamera*>			m_Cameras;

	class CCamera* m_pCurrCamera = nullptr;
	const _tchar* m_pCurrCameraTag = nullptr;
		


public:
	virtual void Free() override;
};

END