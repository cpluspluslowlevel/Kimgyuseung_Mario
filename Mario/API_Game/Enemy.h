#ifndef _API_GAME_ENEMY_H_
#define _API_GAME_ENEMY_H_

#include "Global.h"
#include "Time.h"
#include "Unit.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

//적들의 베이스 클래스입니다.
//적들이 필요로하는 기본적인 데이터, 행동들을 가지고 있습니다.
class CEnemy_Base : public Game_Object::Unit::CUnit{
public:

	struct SPRITE_SET{

		CBitmap_Base* pbitmap;
		const SPRITE* pwalk_sprite;
		const SPRITE* pstun_sprite;
		const SPRITE* pdie_push_sprite;
		const SPRITE* pdie_damage_sprite;

		//쿠파 전용입니다.
		const SPRITE* pfire_sprite;
		const SPRITE* pjump_sprite;
		const SPRITE* ppush_sprite;

		DWORD dwanimation_speed_ms;

	};

public:
	CEnemy_Base();
	CEnemy_Base( const CEnemy_Base& ) = default;
	CEnemy_Base( CEnemy_Base&& ) = default;
	~CEnemy_Base() = default;
	CEnemy_Base& operator=( const CEnemy_Base& ) = default;
	CEnemy_Base& operator=( CEnemy_Base&& ) = default;

	//CGame_Object override;
	virtual Update_Result update() override;

	void initialize();

	void set_sprite_set( const SPRITE_SET& sprite_set );

protected:

	Ability::CBitmap_Render_Ability m_render;
	Ability::CAI_Movement			m_movement;

	SPRITE_SET m_sprite_set;

};

class CGoomba : public CEnemy_Base{
public:

	enum class EState{
		Walk,				//이동
		Die_Push,
		Die_Damage
	};

public:
	CGoomba();
	CGoomba( const CGoomba& ) = default;
	CGoomba( CGoomba&& ) = default;
	~CGoomba() = default;
	CGoomba& operator=( const CGoomba& ) = default;
	CGoomba& operator=( CGoomba&& ) = default;

	virtual Update_Result update() override;

private:

	void push();
	void die_damage();

	EState m_state;

};

class CTurtle : public CEnemy_Base{
public:

	enum class EState{
		Walk,				//이동
		Stun,				//충격을 받아 껍질상태
		Stun_Walk,			//껍질 상태로 이동
		Die_Damage
	};

public:
	CTurtle();
	CTurtle( const CTurtle& ) = default;
	CTurtle( CTurtle&& ) = default;
	~CTurtle() = default;
	CTurtle& operator=( const CTurtle& ) = default;
	CTurtle& operator=( CTurtle&& ) = default;

	virtual Update_Result update() override;

private:

	void push();
	void die_damage();

	void check_damage();

	EState m_state;
	
	Ability::CAI_Movement::EState m_old_movement_state;

};

class CKoopa : public CEnemy_Base{
public:

	enum class EState{
		Spawn,				//등장
		Fire,				//불 발사
		Jump,				//플레이어 위치로 점프
		Push,				//내려 찍기
		Die					//사망
	};

public:
	CKoopa();
	CKoopa( const CKoopa& ) = default;
	CKoopa( CKoopa&& ) = default;
	~CKoopa() = default;
	CKoopa& operator=( const CKoopa& ) = default;
	CKoopa& operator=( CKoopa&& ) = default;

	virtual Update_Result update() override;

	void set_state( EState state );

private:

	void change_state();
	void setting_direction();
	void crush_wall();

	void spawn();
	void fire();
	void jump();
	void push();
	void die();

	EState m_state;

	//Fire
	int m_ifire_count;

	//Jump
	Vector2 m_ptjump_target;

	//Push
	Time::CTimer m_push_stop_timer;
	bool		 m_bpush_old_jump;
	bool         m_bpush_crush_wall;

	//Die
	Time::CTimer m_die_timer;
	float        m_fdie_quake;
	float        m_fdie_quake_direction;

};

class CKoopa_Fire : public CUnit{
public:
	CKoopa_Fire();
	CKoopa_Fire( const CKoopa_Fire& ) = default;
	CKoopa_Fire( CKoopa_Fire&& ) = default;
	~CKoopa_Fire() = default;
	CKoopa_Fire& operator=( const CKoopa_Fire& ) = default;
	CKoopa_Fire& operator=( CKoopa_Fire&& ) = default;

	virtual Update_Result update() override;

private:

	Ability::CBitmap_Render_Ability		 m_render;
	Ability::CDirection_Movement_Ability m_movement;

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif