#pragma once
//#include "../FMOD/include/fmod.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "../Bin/FMOD/fmod.h"
#include "GameObject.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum CHANNELID
	{
		BGM,
		CH1,
		CH2,
		CH3,
		CH4,
		CH5,
		CH6,
		CH7,
		CH8,
		CH9,
		CH10,
		CH11,
		CH12,
		CH13,
		CH14,
		CH15,
		CH16,
		CH17,
		CH18,
		CH19,
		CH20,
		CH21,
		CH22,
		CH23,
		CH24,
		CH25,
		CH26,
		CH27,
		CH28,
		CH29,
		CH30,
		CH31,
		CH32,
		CH33,
		CH34,
		CH35,
		CH36,
		CH37,
		CH38,
		CH39,
		CH40,
		CH_END
	};

	enum CHANNEL_TYPE {
		CHANNEL_BGM,
		CHANNEL_PLAYER,
		CHANNEL_ITEM,
		CHANNEL_GHOST,
		CHANNEL_MONSTER_BULLET,
		CHANNEL_EFFECT,
		CHANNEL_UI,
		//CHANNEL_FX = 1,
		//CHANNEL_ENV = 16,
		CHANNEL_MAX
	};

private:
	CSoundMgr();
	~CSoundMgr();

public:
	HRESULT Initialize();

	void Release();
public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void PlayBGM(TCHAR* pSoundKey, _float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();

	void		PlaySound(const TCHAR* strSoundKey, CHANNEL_TYPE eType, _float fVolume = 1.f);
	void		PlaySoundDistance(const TCHAR* strSoundKey, CHANNEL_TYPE eType,  CGameObject* pTarget, _float fVolume = 1.f);
	void		ChangeBGM(TCHAR* pSoundKey, _float fVolume = 1.f);
	void		Tick();
	CHANNELID	Get_CurChannel(CHANNEL_TYPE etype) { return m_eCurChannel[etype]; }

	void Set_Player(CGameObject* pPlayer)
	{
		m_pPlayer = pPlayer;
	}


private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

	CGameObject* m_pPlayer = nullptr;

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL*	m_pChannelArr[CH_END];
	CHANNELID		m_eCurChannel[CHANNEL_MAX];
	CHANNELID		m_eChannelMaxNum[CHANNEL_MAX];
	CHANNELID		m_eChannelMinNum[CHANNEL_MAX];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM*	m_pSystem;
	_bool			m_bPause = false;

	_float			m_fSound = 0.f;
	_float			m_fSoundMaxDistance = 0.f;
	_float			m_fMaxBGMVolume = 1.f;

	_bool			m_bBGMOut = false;
	TCHAR *			m_NextSound;
};

