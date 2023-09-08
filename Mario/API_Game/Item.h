#ifndef _API_GAME_ITEM_H_
#define _API_GAME_ITEM_H_

#include "Global.h"
#include "Time.h"
#include "Unit.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

//아이템들의 베이스 클래스입니다.
//기본적인 알고리즘의 뼈대를 제공하고 이 클래스를 상속받아 부분부분을 확장하는 형태입니다.
class CItem_Base : public Game_Object::Unit::CUnit{
public:
	CItem_Base();
	CItem_Base( const CItem_Base& ) = default;
	CItem_Base( CItem_Base&& ) = default;
	~CItem_Base() = default;
	CItem_Base& operator=( const CItem_Base& ) = default;
	CItem_Base& operator=( CItem_Base&& ) = default;

	enum class EState{
		Spawn,				//스폰중
		Move				//이동
	};

public:

	//CGame_Object override;
	virtual Update_Result update() override;

	//My virtual
	virtual void hit_player( Ability::CAbility_Data* pability ) = 0;

protected:

	//My virtual
	virtual void update_spawn() = 0;
	virtual void update_move() = 0;

	void set_state( EState state );

	EState get_state() const;

	Ability::CBitmap_Render_Ability m_render;

private:

	EState m_state;

	Time::CTimer m_spawn_timer;

};

class CMushroom_Item : public CItem_Base{
public:
	CMushroom_Item();
	CMushroom_Item( const CMushroom_Item& ) = default;
	CMushroom_Item( CMushroom_Item&& ) = default;
	~CMushroom_Item() = default;
	CMushroom_Item& operator=( const CMushroom_Item& ) = default;
	CMushroom_Item& operator=( CMushroom_Item&& ) = default;

	//CItem_Base override
	virtual void hit_player( Ability::CAbility_Data* pability );

protected:

	//CItem_Base override
	virtual void update_spawn();
	virtual void update_move();

private:

	Ability::CX_Direction_Movement m_movement;

};

class CCoin : public CItem_Base{
public:
	CCoin();
	CCoin( const CCoin& ) = default;
	CCoin( CCoin&& ) = default;
	~CCoin() = default;
	CCoin& operator=( const CCoin& ) = default;
	CCoin& operator=( CCoin&& ) = default;

	//CItem_Base override
	virtual void hit_player( Ability::CAbility_Data* pability );

protected:

	//CItem_Base override
	virtual void update_spawn();
	virtual void update_move();


};

class CGoal_Item : public CItem_Base{
public:
	CGoal_Item();
	CGoal_Item( const CGoal_Item& ) = default;
	CGoal_Item( CGoal_Item&& ) = default;
	~CGoal_Item() = default;
	CGoal_Item& operator=( const CGoal_Item& ) = default;
	CGoal_Item& operator=( CGoal_Item&& ) = default;

	//CItem_Base override
	virtual void hit_player( Ability::CAbility_Data* pability );

protected:

	//CItem_Base override
	virtual void update_spawn();
	virtual void update_move();


};


API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif