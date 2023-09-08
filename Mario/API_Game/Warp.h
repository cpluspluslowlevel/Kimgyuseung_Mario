#ifndef _API_GAME_WARP_H_
#define _API_GAME_WARP_H_

#include "Global.h"

#include "Unit.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

class CWarp : public CUnit{
public:
	CWarp();
	CWarp( const CWarp& ) = default;
	CWarp( CWarp&& ) noexcept = default;
	~CWarp() = default;
	CWarp& operator=( const CWarp& ) = default;
	CWarp& operator=( CWarp&& ) noexcept = default;

	//CGame_Object override;
	virtual Update_Result update() override;

	void set_stage( DWORD dwstage );
	void set_location( DWORD dwlocation );

	DWORD get_stage() const;
	DWORD get_location() const;

private:

	DWORD m_dwstage;
	DWORD m_dwlocation;

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif