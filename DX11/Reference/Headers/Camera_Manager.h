#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

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