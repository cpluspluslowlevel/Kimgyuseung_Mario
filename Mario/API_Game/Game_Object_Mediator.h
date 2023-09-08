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

//���ӿ�����Ʈ�鰣 ��ȣ�ۿ��� ����մϴ�.
//��κ� �����Լ���� �����Ǹ� ��ä�鰣 N:N���踦 ��ü�� �߰����� N:1�� �ܼ�ȭ �մϴ�.
//��ü�鰣 ��ȣ�ۿ��� ��ü���� ������ ��� ���� �������� ���谡 �˴ϴ�. �׷��� 
//�� �߰��� Ŭ������ ��ȣ�� ������ ����ϰ� ��ü���� �� �߰��� Ŭ������ ���Ͽ�
//��ȣ ����մϴ�.
//�аԴ� ���ֻ���ϴ� �Լ��鵵 ������ �ֽ��ϴ�.
class CGame_Object_Manager::CMediator{
public:

	//�÷��̾ ����ϴ�.
	//position: ��ġ
	//direction: ����
	static std::weak_ptr< CGame_Object > create_player( const Vector2& position, const Vector2& direction );

	//���� �����մϴ�.
	//position: ��ġ
	//scale: ũ��
	//tile: Ÿ���� ����
	//return: ������ ������Ʈ�� ������
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

	//����Ʈ�� �����մϴ�.
	//position: ��ġ
	//scale: ũ��
	template< typename T >
	static std::weak_ptr< CGame_Object > create_effect( const Vector2& position, const Vector2& scale ){

		Unit::CEffect_Base* peffect = new T{};
		peffect->get_position()->set_position( position );
		peffect->get_position()->set_scale( scale );

		std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( peffect, CGame_Object_Manager::EList_Type::Effect );

		return ptr;

	}

	//�������� �����մϴ�.
	//position: ��ġ
	//scale: ũ��
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

	//���� �����մϴ�.
	//position: ��ġ
	//tile: Ÿ���� ����
	static std::weak_ptr< CGame_Object > create_enemy( const Vector2& position, const Tile_Map::TILE& tile );
	static void                          setting_enemy_animation( Unit::CEnemy_Base* penemy, const Tile_Map::TILE& tile );

	//�������� �����մϴ�.
	//position: ��ġ
	//tile: Ÿ���� ����
	static std::weak_ptr< CGame_Object > create_warp( const Vector2& position, const Tile_Map::TILE& tile );

	//Ÿ�ϸʿ� ������ ������Ʈ���� �����մϴ�.
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



	//�� �簢���� �浹�� �����մϴ�.
	//position0, scale0: �簢���� ��ġ�� ũ���Դϴ�.
	//position1, scale1: �Ǵٸ� �簢���� ��ġ�� ũ���Դϴ�.
	//return: �浹������ true, �ƴϸ� false
	static bool intersect_rect( const Vector2& position0, const Vector2& scale0,
								const Vector2& position1, const Vector2& scale1 );
	static bool intersect_rect( const Ability::CPosition_Ability* pposition0, const Ability::CPosition_Ability* pposition1 );
	static bool intersect_rect( const Ability::CAbility_Data* pability0, const Ability::CAbility_Data* pability1 );
	static bool intersect_rect_x( const Ability::CAbility_Data* prect_ability0, const Ability::CAbility_Data* pability );


	//������ ȭ�� ������ �������� �˻��մϴ�.
	//pposition: ��ġ
	//scale: ũ��
	//return: ������ �Ѿ���� true. �ƴϸ� false
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