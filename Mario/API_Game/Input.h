#ifndef _MARIO_TOOL_INPUT_H_
#define _MARIO_TOOL_INPUT_H_

#include "Global.h"

#include <Windows.h>

struct KEYSTATE{
	bool bpush;		//ó�� ���� ������ true�Դϴ�.
	bool bdown;     //������ �׻� true�Դϴ�.
	bool bup;		//ó�� ��ư�� ������ ���� true�Դϴ�.
};

class CInput{
	SINGLETON_DEFAULT( CInput );
public:

	void update();

	void insert( DWORD dwKey );

	KEYSTATE get_state( DWORD dwKey );


private:

	std::map< DWORD, KEYSTATE > m_key_state_map;

};

#define INPUT Library::CSingleton< CInput >::get()

#endif