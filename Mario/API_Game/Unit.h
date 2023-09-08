#ifndef _RPG_UNIT_H_
#define _RPG_UNIT_H_

#include "Game_Object.h"
#include "Ability.h"


API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

enum class EType{
	Player,
	Enemy,
	Item,
	NPC
};

//게임에서 움직이는 오브젝트들이 가지는 기본 클래스 입니다.
class CUnit : public CGame_Object{
public:

	static const Update_Result UPDATE_RESULT_UNIT_BEGIN	= UPDATE_RESULT_GAME_OBJECT_END + 1;
	static const Update_Result UPDATE_RESULT_UNIT_END	= UPDATE_RESULT_UNIT_BEGIN + 100;

public:
	CUnit();
	CUnit( const CUnit& ) = default;
	CUnit( CUnit&& ) = default;
	virtual ~CUnit() = default;
	CUnit& operator=( const CUnit& ) = default;
	CUnit& operator=( CUnit&& ) = default;

	Ability::CPosition_Ability* get_position();

private:

	//Ability
	Ability::CPosition_Ability m_position;

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif