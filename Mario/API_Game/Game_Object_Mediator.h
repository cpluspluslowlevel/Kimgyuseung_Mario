#ifndef _API_GAME_GAME_OBJECT_MEDIATOR_H_
#define _API_GAME_GAME_OBJECT_MEDIATOR_H_

#include "Global.h"
#include "Math.h"
#include "Tile.h"
#include "Game_Object.h"
#include "Ability.h"
#include "Player.h"
#include "Wall.h"
#include "Effect.h"
#include "Item.h"
#include "Enemy.h"
#include "Warp.h"


API_GAME_GAME_OBJECT_NAMESPACE_BEGIN

class CScroll;

//게임오브젝트들간 상호작용을 담당합니다.
//대부분 정적함수들로 구성되며 객채들간 N:N관계를 객체와 중개자의 N:1로 단순화 합니다.
//객체들간 상호작용을 객체들이 직접할 경우 서로 종속적인 관계가 됩니다. 그래서 
//이 중개자 클래스가 상호간 연결을 담당하고 객체들은 이 중개자 클래스를 통하여
//상호 통신합니다.
//넓게는 자주사용하는 함수들도 가지고 있습니다.
class CGame_Object_Manager::CMediator{
public:

	//플레이어를 만듭니다.
	//position: 위치
	//direction: 방향
	static std::weak_ptr< CGame_Object > create_player( const Vector2& position, const Vector2& direction );

	//벽을 생성합니다.
	//position: 위치
	//scale: 크기
	//tile: 타일의 정보
	//return: 생성된 오브젝트의 포인터
	template< typename T >
	static std::weak_ptr< CGame_Object > create_wall( const Vector2& position, const Vector2& scale, const Tile_Map::TILE& tile ){

		Unit::CTile* ptile = new T{};
		ptile->set_tile( tile );
		ptile->get_position()->set_position( position );
		ptile->get_position()->set_scale( scale );

		std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( ptile, CGame_Object_Manager::EList_Type::Wall );

		GAME_OBJECT_MANAGER->get_wall_group()->insert( ptr );

		return ptr;

	}

	//이펙트를 생성합니다.
	//position: 위치
	//scale: 크기
	template< typename T >
	static std::weak_ptr< CGame_Object > create_effect( const Vector2& position, const Vector2& scale ){

		Unit::CEffect_Base* peffect = new T{};
		peffect->get_position()->set_position( position );
		peffect->get_position()->set_scale( scale );

		std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( peffect, CGame_Object_Manager::EList_Type::Effect );

		return ptr;

	}

	//아이템을 생성합니다.
	//position: 위치
	//scale: 크기
	template< typename T >
	static std::weak_ptr< CGame_Object > create_item( const Vector2& position, const Vector2& scale ){

		Unit::CItem_Base* pitem = new T{};
		pitem->get_position()->set_position( position );
		pitem->get_position()->set_scale( scale );
		Ability::setting_normal( pitem->get_ability_data() );

		std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( pitem, CGame_Object_Manager::EList_Type::Unit );

		GAME_OBJECT_MANAGER->get_item_group()->insert( ptr );

		return ptr;

	}

	//적을 생성합니다.
	//position: 위치
	//tile: 타일의 정보
	static std::weak_ptr< CGame_Object > create_enemy( const Vector2& position, const Tile_Map::TILE& tile );
	static void                          setting_enemy_animation( Unit::CEnemy_Base* penemy, const Tile_Map::TILE& tile );

	//파이프를 생성합니다.
	//position: 위치
	//tile: 타일의 정보
	static std::weak_ptr< CGame_Object > create_warp( const Vector2& position, const Tile_Map::TILE& tile );

	//타일맵에 설정된 오브젝트들을 생성합니다.
	static bool create_tile_map_object();

	static Tile_Map::ECollision_Type get_collition_type( const XY& sprite_index );
	static Tile_Map::EAttribute      get_create_enemy_type( const Tile_Map::TILE& tile );
	static Tile_Map::EAttribute      get_position_attribute(const Tile_Map::TILE& tile);
	static Tile_Map::EAttribute      get_stage_attribute( const Tile_Map::TILE& tile );
	static bool						 is_image_tile( const XY& sprite_index );
	static bool                      is_enemy_tile( const Tile_Map::TILE& tile );
	static bool                      is_pipe_tile( const Tile_Map::TILE& tile );
	static bool                      is_coin_tile( const Tile_Map::TILE& tile );

	static bool is_sliding_tile( const Tile_Map::TILE_INDEX& index );



	//두 사각형의 충돌을 검출합니다.
	//position0, scale0: 사각형의 위치와 크기입니다.
	//position1, scale1: 또다른 사각형의 위치와 크기입니다.
	//return: 충돌했으면 true, 아니면 false
	static bool intersect_rect( const Vector2& position0, const Vector2& scale0,
								const Vector2& position1, const Vector2& scale1 );
	static bool intersect_rect( const Ability::CPosition_Ability* pposition0, const Ability::CPosition_Ability* pposition1 );
	static bool intersect_rect( const Ability::CAbility_Data* pability0, const Ability::CAbility_Data* pability1 );
	static bool intersect_rect_x( const Ability::CAbility_Data* prect_ability0, const Ability::CAbility_Data* pability );


	//윈도우 화면 밖으로 나갔는지 검사합니다.
	//pposition: 위치
	//scale: 크기
	//return: 밖으로 넘어갔으면 true. 아니면 false
	static bool is_screen_out( const Ability::CPosition_Ability* pposition, const Vector2& scale );


	static Unit::CPlayer* get_player();
	static CScroll* get_scroll();

private:
	static Unit::CPlayer* s_pplayer;
	static CScroll* s_pscroll;

};

class CScroll{
public:
	CScroll();
	CScroll( const CScroll& ) = delete;
	CScroll( CScroll&& ) = delete;
	~CScroll() = default;

	void set_pt( const Vector2& pt );
	void set_delta( const Vector2& pt );

	Vector2 get_pt() const;

private:
	
	Vector2 m_pt;
	Vector2 m_ptdelta;

};

API_GAME_GAME_OBJECT_NAMESPACE_END

#endif