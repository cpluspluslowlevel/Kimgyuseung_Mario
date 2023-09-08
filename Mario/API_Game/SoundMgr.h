#pragma once

#include <fmod.h>

#include "Global.h"

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
	enum CHANNELID {BGM, PLAYER, MONSTER, EFFECT, UI, MAXCHANNEL};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 

	void play_sound( const TCHAR* skey, CHANNELID channel );
	void play_bgm( const TCHAR* skey );
	void StopSound( CHANNELID channel );
	void StopAll();

	bool load( const char* sfile_path, const TCHAR* key );

private:
	static CSoundMgr* m_pInstance; 

	// 사운드 리소스 정보를 갖는 객체 
	std::map< const TCHAR*, FMOD_SOUND* > m_mapSound; 

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

