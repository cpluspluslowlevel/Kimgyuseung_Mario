#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_NAMESPACE_USING
API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_USING
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

Unit::CPlayer* CGame_Object_Manager::CMediator::s_pplayer = nullptr;
CScroll*	   CGame_Object_Manager::CMediator::s_pscroll = new CScroll{};

std::weak_ptr< CGame_Object > CGame_Object_Manager::CMediator::create_player( const Vector2& position, const Vector2& direction ){

	s_pplayer = new CPlayer{};
	s_pplayer->get_position()->set_position( position );
	s_pplayer->get_position()->set_direction( direction );
	std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( s_pplayer, CGame_Object_Manager::EList_Type::Player );

	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( ptr );
	GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->insert( ptr );
	GAME_OBJECT_MANAGER->get_attack_group()->insert( ptr );

	return ptr;

}

std::weak_ptr< CGame_Object > CGame_Object_Manager::CMediator::create_enemy( const Vector2& position, const Tile_Map::TILE& tile ){

	Unit::CEnemy_Base* penemy = nullptr;
	switch( get_create_enemy_type( tile ) ){
	case Tile_Map::EAttribute::Enemy_Goomba:
		penemy = new Unit::CGoomba;
		break;
	case Tile_Map::EAttribute::Enemy_Turtle:
		penemy = new Unit::CTurtle;
		break;
	case Tile_Map::EAttribute::Enemy_Koopa:
		penemy = new Unit::CKoopa;
		break;
	default:
		assert( false );
	}
	penemy->get_position()->set_position( position );

	setting_enemy_animation( penemy, tile );
	penemy->initialize();

	Ability::setting_normal( penemy->get_ability_data() );

	std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( penemy, CGame_Object_Manager::EList_Type::Enemy );

	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( ptr );
	GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->insert(ptr);
	GAME_OBJECT_MANAGER->get_push_group()->insert( ptr );
	GAME_OBJECT_MANAGER->get_hit_group()->insert( ptr );

	return ptr;

}

std::weak_ptr< CGame_Object > CGame_Object_Manager::CMediator::create_warp( const Vector2& position, const Tile_Map::TILE& tile ){

	Unit::CWarp* pwarp = new CWarp{};

	Tile_Map::EAttribute position_attribute = get_position_attribute( tile );
	Tile_Map::EAttribute stage_attribute    = get_stage_attribute( tile );

	pwarp->set_stage( ( DWORD )stage_attribute );
	pwarp->set_location( ( DWORD )position_attribute );
	pwarp->get_position()->set_position( position );

	std::shared_ptr< CGame_Object > ptr = GAME_OBJECT_MANAGER->insert( pwarp, CGame_Object_Manager::EList_Type::Unit );

	return ptr;

}


void CGame_Object_Manager::CMediator::setting_enemy_animation( Unit::CEnemy_Base* penemy, const Tile_Map::TILE& tile ){

	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };

	Unit::CEnemy_Base::SPRITE_SET sprite_set;
	for( auto attribute : attribute_array ){

		switch( attribute ){
		case Tile_Map::EAttribute::Enemy_Goomba:
			sprite_set.pbitmap				= BITMAP_MANAGER->find_bitmap( TEXT( "Enemy_1x1" ) );
			sprite_set.pwalk_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Goomba_Walk" ) );
			sprite_set.pstun_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Goomba_Walk" ) );
			sprite_set.pdie_push_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Goomba_Die_Push" ) );
			sprite_set.pdie_damage_sprite	= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Goomba_Die_Damage" ) );
			sprite_set.dwanimation_speed_ms = 250;
			break;
		case Tile_Map::EAttribute::Enemy_Turtle:
			sprite_set.pbitmap				= BITMAP_MANAGER->find_bitmap( TEXT( "Enemy_1x2" ) );
			sprite_set.pwalk_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Turtle_Walk" ) );
			sprite_set.pstun_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Turtle_Stun" ) );
			sprite_set.pdie_push_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Turtle_Stun" ) );
			sprite_set.pdie_damage_sprite	= BITMAP_MANAGER->find_sprite( TEXT( "Enemy_Turtle_Die_Damage" ) );
			sprite_set.dwanimation_speed_ms = 250;
			break;
		case Tile_Map::EAttribute::Enemy_Koopa:
			sprite_set.pbitmap				= BITMAP_MANAGER->find_bitmap( TEXT( "Koopa" ) );
			sprite_set.pwalk_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Koopa_Walk" ) );
			sprite_set.pfire_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Koopa_Fire" ) );
			sprite_set.pjump_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Koopa_Jump" ) );
			sprite_set.ppush_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Koopa_Push" ) );
			sprite_set.dwanimation_speed_ms = 250;
			break;
		}

	}
	penemy->set_sprite_set( sprite_set );



}

bool CGame_Object_Manager::CMediator::create_tile_map_object(){

	Vector2						vtile_size		= TILE_MAP->get_tile_property().vtile_size;
	const Tile_Map::TILE_INDEX& vtile_map_size	= TILE_MAP->get_tile_map_property().tile_map_size;

	//타일을 순회하며 정보를 살펴봅니다.
	for( size_t iy = 0; iy < ( size_t )vtile_map_size.iy; ++iy ){
		for( size_t ix = 0; ix < ( size_t )vtile_map_size.ix; ++ix ){

			Vector2 pttile{ ( ( float )ix + 0.5f ) * vtile_size.fx, ( ( float )iy + 0.5f ) * vtile_size.fy };

			Tile_Map::TILE tile = TILE_MAP->get_tile( { ( int )ix, ( int )iy } );
			tile.collision_type = get_collition_type( tile.sprite_index );

			//타일 정보에 기반하여 객체를 생성합니다.
			if( tile.collision_type == Tile_Map::ECollision_Type::Quad ){
				
				if( tile.sprite_index.ix == 67  && tile.sprite_index.iy == 2 ){
					create_wall< Game_Object::Unit::CItem_Wall >( pttile, vtile_size, tile );
				}else{
					create_wall< Game_Object::Unit::CWall >( pttile, vtile_size, tile );
				}

			}else if( tile.collision_type == Tile_Map::ECollision_Type::Quad_Up ){
				create_wall< Game_Object::Unit::CQuad_Up_Wall >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Quad_Crushable ){
				create_wall< Game_Object::Unit::CCrushable_Wall >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Right ){
				create_wall< Game_Object::Unit::CDiagonal_Right >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Right_Bottom ){
				create_wall< Game_Object::Unit::CDiagonal_Right_Bottom >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Right_Middle ){
				create_wall< Game_Object::Unit::CDiagonal_Right_Middle >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Left ){
				create_wall< Game_Object::Unit::CDiagonal_Left >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Left_Bottom ){
				create_wall< Game_Object::Unit::CDiagonal_Left_Bottom >( pttile, vtile_size, tile );
			}else if( tile.collision_type == Tile_Map::ECollision_Type::Diagonal_Left_Middle ){
				create_wall< Game_Object::Unit::CDiagonal_Left_Middle >( pttile, vtile_size, tile );
			}else if( is_coin_tile( tile ) ){
				create_item< CCoin >( pttile, vtile_size );
			}else{
				
				if( is_image_tile( tile.sprite_index ) ){
					create_wall< CTile >( pttile, vtile_size, tile );
				}

				//골인 타일이면 골인 오브젝트를 생성합니다.
				if( tile.attribute0 == Tile_Map::EAttribute::Goal ){
					create_item< CGoal_Item >( pttile + vtile_size * 0.5f, vtile_size );
				}

				if( is_enemy_tile( tile ) ){
					create_enemy( pttile, tile );
				}

				if( is_pipe_tile( tile ) ){
					create_warp( pttile, tile );
				}

			}

		}
	}

	return true;

}

Tile_Map::ECollision_Type CGame_Object_Manager::CMediator::get_collition_type( const XY& sprite_index ){

	if( sprite_index.ix >= 26  && sprite_index.ix <= 28 &&
		sprite_index.iy >= 9  && sprite_index.iy <= 11 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix == 61 && sprite_index.iy == 8 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix == 62 &&
		sprite_index.iy >= 8  && sprite_index.iy <= 13 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix >= 30  && sprite_index.ix <= 31 &&
		sprite_index.iy == 20 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix >= 23  && sprite_index.ix <= 25 &&
		sprite_index.iy >= 15  && sprite_index.iy <= 16 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix == 23 && sprite_index.iy == 17 ){
		return Tile_Map::ECollision_Type::Diagonal_Right_Bottom;
	}

	if( sprite_index.ix == 24 && sprite_index.iy == 17 ){
		return Tile_Map::ECollision_Type::Diagonal_Right_Middle;
	}

	if( sprite_index.ix == 25 && sprite_index.iy == 17 ){
		return Tile_Map::ECollision_Type::Diagonal_Right;
	}

	if( sprite_index.ix == 23 && sprite_index.iy == 18 ){
		return Tile_Map::ECollision_Type::Diagonal_Left_Middle;
	}

	if( sprite_index.ix == 24 && sprite_index.iy == 18 ){
		return Tile_Map::ECollision_Type::Diagonal_Left_Bottom;
	}

	if( sprite_index.ix == 25 && sprite_index.iy == 18 ){
		return Tile_Map::ECollision_Type::Diagonal_Left;
	}

	if( sprite_index.ix >= 0  && sprite_index.ix <= 7 &&
		sprite_index.iy >= 2  && sprite_index.iy <= 13 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix == 67  && sprite_index.iy == 2 ){
		return Tile_Map::ECollision_Type::Quad;
	}

	if( sprite_index.ix == 59  && sprite_index.iy == 1 ){
		return Tile_Map::ECollision_Type::Quad;
	}
	
	if( sprite_index.ix == 61  && sprite_index.iy == 0 ){
		return Tile_Map::ECollision_Type::Quad_Crushable;
	}

	if( sprite_index.ix == 46  && sprite_index.iy == 9 ){
		return Tile_Map::ECollision_Type::Quad_Crushable;
	}

	if( sprite_index.ix >= 54  && sprite_index.ix <= 59 ){

		if( sprite_index.iy == 8 || sprite_index.iy == 11 ){
			return Tile_Map::ECollision_Type::Quad_Up;
		}

	}

	return Tile_Map::ECollision_Type::None;

}

Tile_Map::EAttribute CGame_Object_Manager::CMediator::get_create_enemy_type( const Tile_Map::TILE& tile ){

	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };
	for( auto attribute : attribute_array ){

		if( attribute == Tile_Map::EAttribute::Enemy_Goomba ||
			attribute == Tile_Map::EAttribute::Enemy_Turtle ||
			attribute == Tile_Map::EAttribute::Enemy_Koopa ){
			return attribute;
		}

	}

	return Tile_Map::EAttribute::None;
}

Tile_Map::EAttribute CGame_Object_Manager::CMediator::get_position_attribute( const Tile_Map::TILE& tile ){
	
	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };
	for( auto attribute : attribute_array ){

		if( attribute == Tile_Map::EAttribute::Position_0 ||
			attribute == Tile_Map::EAttribute::Position_1 ||
			attribute == Tile_Map::EAttribute::Position_2 ||
			attribute == Tile_Map::EAttribute::Position_3 ||
			attribute == Tile_Map::EAttribute::Position_4 ||
			attribute == Tile_Map::EAttribute::Position_5 ||
			attribute == Tile_Map::EAttribute::Position_6 ||
			attribute == Tile_Map::EAttribute::Position_7 ||
			attribute == Tile_Map::EAttribute::Position_8 ||
			attribute == Tile_Map::EAttribute::Position_9  ){
			return attribute;
		}

	}

	return Tile_Map::EAttribute::None;

}


Tile_Map::EAttribute CGame_Object_Manager::CMediator::get_stage_attribute( const Tile_Map::TILE& tile ){
	
	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };
	for( auto attribute : attribute_array ){

		if( attribute == Tile_Map::EAttribute::Stage_0 ||
			attribute == Tile_Map::EAttribute::Stage_1 ||
			attribute == Tile_Map::EAttribute::Stage_2 ||
			attribute == Tile_Map::EAttribute::Stage_3 ||
			attribute == Tile_Map::EAttribute::Stage_4 ||
			attribute == Tile_Map::EAttribute::Bonus_0 ||
			attribute == Tile_Map::EAttribute::Bonus_1 ||
			attribute == Tile_Map::EAttribute::Bonus_2 ||
			attribute == Tile_Map::EAttribute::Bonus_3 ||
			attribute == Tile_Map::EAttribute::Bonus_4 ){
			return attribute;
		}

	}

	return Tile_Map::EAttribute::None;

}

bool CGame_Object_Manager::CMediator::is_image_tile( const XY& sprite_index ){
	return !( sprite_index.ix >= 38 && sprite_index.ix <= 41 &&
			  sprite_index.iy >= 1 && sprite_index.iy <= 2 );
}

bool CGame_Object_Manager::CMediator::is_enemy_tile( const Tile_Map::TILE& tile ){

	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };

	bool breturn = false;
	for( auto attribute : attribute_array ){

		if( attribute == Tile_Map::EAttribute::Enemy_Goomba ||
			attribute == Tile_Map::EAttribute::Enemy_Turtle ||
			attribute == Tile_Map::EAttribute::Enemy_Koopa ){
			breturn = true;
		}

	}

	return breturn;

}

bool CGame_Object_Manager::CMediator::is_pipe_tile( const Tile_Map::TILE& tile ){

	Tile_Map::EAttribute attribute_array[] = { tile.attribute0, tile.attribute1, tile.attribute2, tile.attribute3 };

	bool breturn = false;
	for( auto attribute : attribute_array ){

		if( attribute == Tile_Map::EAttribute::Pipe ){
			breturn = true;
		}

	}

	return breturn;

}

bool CGame_Object_Manager::CMediator::is_coin_tile( const Tile_Map::TILE& tile ){
	return tile.sprite_index.ix == 56 && tile.sprite_index.iy == 4;
}

bool CGame_Object_Manager::CMediator::is_sliding_tile( const Tile_Map::TILE_INDEX& index ){

	if( !TILE_MAP->is_available_index( index ) ) return false;

	std::weak_ptr< Game_Object::CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( index );
	if( pweak_tile.expired() ) return false;

	auto ptile = pweak_tile.lock();
	if( ( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Left ||
		( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Left_Bottom ||
		( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Left_Middle ||
		( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Right ||
		( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Right_Bottom ||
		( ( CTile* )ptile.get() )->get_tile().collision_type == Tile_Map::ECollision_Type::Diagonal_Right_Middle ){
		return true;
	}

	return false;

}

bool CGame_Object_Manager::CMediator::intersect_rect( const Vector2& position0, const Vector2& scale0,
													  const Vector2& position1, const Vector2& scale1 ){
	if( position0.fx + scale0.fx * 0.5f < position1.fx - scale1.fx * 0.5f ) return false;
	if( position0.fx - scale0.fx * 0.5f > position1.fx + scale1.fx * 0.5f ) return false;
	if( position0.fy + scale0.fy * 0.5f < position1.fy - scale1.fy * 0.5f ) return false;
	if( position0.fy - scale0.fy * 0.5f > position1.fy + scale1.fy * 0.5f ) return false;
	return true;
}

bool CGame_Object_Manager::CMediator::intersect_rect( const Ability::CPosition_Ability* pposition0, const Ability::CPosition_Ability* pposition1 ){
	return intersect_rect( pposition0->get_position(), pposition0->get_scale(),
						   pposition1->get_position(), pposition1->get_scale() );
}

bool CGame_Object_Manager::CMediator::intersect_rect( const Ability::CAbility_Data* pability0, const Ability::CAbility_Data* pability1 ){
	return intersect_rect( pability0->get_position()->position, pability0->get_position()->vscaling,
						   pability1->get_position()->position, pability1->get_position()->vscaling );
}

bool CGame_Object_Manager::CMediator::intersect_rect_x(const Ability::CAbility_Data* prect_ability0, const Ability::CAbility_Data* pability ){
	if( prect_ability0->get_position()->left() > pability->get_position()->right() ) return false;
	if( prect_ability0->get_position()->right() < pability->get_position()->left() ) return false;
	return true;
}

bool CGame_Object_Manager::CMediator::is_screen_out( const Ability::CPosition_Ability* pposition, const Vector2& scale ){

	Vector2 pt = pposition->get_position();
	if( pt.fx + scale.fx * 0.5f < 0.0f )   return true;
	if( pt.fx - scale.fx * 0.5f > 800.0f ) return true;
	if( pt.fy + scale.fy * 0.5f < 0.0f )   return true;
	if( pt.fy - scale.fy * 0.5f > 600.0f ) return true;

	return false;

}

CPlayer* CGame_Object_Manager::CMediator::get_player(){
	return s_pplayer;
}

CScroll* CGame_Object_Manager::CMediator::get_scroll(){
	return s_pscroll;
}

CScroll::CScroll() : m_pt{ 0.0f, 0.0f },
					 m_ptdelta{ 0.0f, 0.0f }{
}

void CScroll::set_pt( const Vector2& pt ){
	
	m_pt = pt;
	m_pt.fx -= 800.0f * 0.5f;
	m_pt.fy -= 600.0f * 0.5f;

	Vector2 vmap_size = TILE_MAP->get_map_size();

	if( m_pt.fx < 0.0f ){
		m_pt.fx = 0.0f;
	}

	if( m_pt.fx + 800.0f > vmap_size.fx ){
		m_pt.fx = vmap_size.fx - 800.0f;
	}

	if( m_pt.fy < 0.0f ){
		m_pt.fy = 0.0f;
	}

	if( m_pt.fy + 600.0f > vmap_size.fy ){
		m_pt.fy = vmap_size.fy - 600.0f;
	}

	//UI를 위한 공간입니다.
	m_pt.fy += 100.0f;


}

void CScroll::set_delta( const Vector2& pt ){
	m_ptdelta = pt;
}

Vector2 CScroll::get_pt() const{
	return m_pt + m_ptdelta;
}