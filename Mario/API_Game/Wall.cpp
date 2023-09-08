#include "Wall.h"

#include "SoundMgr.h"
#include "Input.h"
#include "Effect.h"
#include "Item.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

CTile::CTile() : m_tile_render{},
				 m_physics{},
				 m_tile{},
				 m_bleft_check{ true },
				 m_bright_check{ true },
				 m_btop_check{ true },
				 m_bbottom_check{ true }{

	get_ability_data()->alloc_rendering();
	get_ability_data()->alloc_physics();

	get_position()->set_data( get_ability_data() );
	m_physics.set_data( get_ability_data() );
	m_tile_render.set_data( get_ability_data() );

	m_tile_render.set_data( get_ability_data() );
	get_ability_data()->get_rendering()->pbitmap	= BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) );
	get_ability_data()->get_rendering()->color_key	= RGB( 255, 0, 255 );
	get_ability_data()->get_rendering()->animator.set_bitmap( m_tile_render.get_data()->get_rendering()->pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Tile" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 200 );

}

Game_Object::CGame_Object::Update_Result CTile::update(){
	return get_update_result();
}

void CTile::start(){
}

void CTile::collision( Game_Object::Ability::CAbility_Data* pdata ){
}

bool CTile::push_crush(){
	return false;
}

void CTile::render(){
	m_tile_render.render();
	if( INPUT->get_state( VK_RETURN ).bdown ){
		//m_tile_render.render_quad();
	}
}

bool CTile::is_left_wall(){
	return false;
}

bool CTile::is_right_wall(){
	return false;
}

bool CTile::is_top_wall(){
	return false;
}

bool CTile::is_down_wall(){
	return false;
}

Vector2 CTile::get_direction() const{
	return { 1.0f, 0.0f };
}

void CTile::setting_check(){

	//주변 인덱스를 구합니다.
	Tile_Map::TILE_INDEX left_index		= { get_tile().index.ix - 1, get_tile().index.iy };
	Tile_Map::TILE_INDEX right_index	= { get_tile().index.ix + 1, get_tile().index.iy };
	Tile_Map::TILE_INDEX top_index		= { get_tile().index.ix, get_tile().index.iy - 1 };
	Tile_Map::TILE_INDEX bottom_index	= { get_tile().index.ix, get_tile().index.iy + 1 };

	//유효한 인덱스고 그 방향으로 현재 타일이 막는다면 그 방향으로는 충돌처리 하지 말라고 알려줍니다.
	if( TILE_MAP->is_available_index( left_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( left_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_right_check( !is_left_wall() );
		}

	}

	if( TILE_MAP->is_available_index( right_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( right_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_left_check( !is_right_wall() );
		}

	}

	if( TILE_MAP->is_available_index( top_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( top_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_bottom_check( !is_top_wall() );
		}

	}

	if( TILE_MAP->is_available_index( bottom_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( bottom_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_top_check( !is_down_wall() );
		}

	}

}

void CTile::reset_check(){

	//주변 인덱스를 구합니다.
	Tile_Map::TILE_INDEX left_index		= { get_tile().index.ix - 1, get_tile().index.iy };
	Tile_Map::TILE_INDEX right_index	= { get_tile().index.ix + 1, get_tile().index.iy };
	Tile_Map::TILE_INDEX top_index		= { get_tile().index.ix, get_tile().index.iy - 1 };
	Tile_Map::TILE_INDEX bottom_index	= { get_tile().index.ix, get_tile().index.iy + 1 };

	//유효한 인덱스면 그 방향으로 충돌하라고 설정 해줍니다.
	if( TILE_MAP->is_available_index( left_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( left_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_right_check( true );
		}

	}

	if( TILE_MAP->is_available_index( right_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( right_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_left_check( true );
		}

	}

	if( TILE_MAP->is_available_index( top_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( top_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_bottom_check( true );
		}

	}

	if( TILE_MAP->is_available_index( bottom_index ) ){

		std::weak_ptr< CGame_Object > pweak_left = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( bottom_index );
		if( !pweak_left.expired() ){
			( ( Unit::CTile* )pweak_left.lock().get() )->set_top_check( true );
		}

	}

}

bool CTile::set_left_check( bool bcheck ){
	return m_bleft_check = bcheck;
}

bool CTile::set_right_check( bool bcheck ){
	return m_bright_check = bcheck;
}

bool CTile::set_top_check( bool bcheck  ){
	return m_btop_check = bcheck;
}

bool CTile::set_bottom_check( bool bcheck ){
	return m_bbottom_check = bcheck;
}

void CTile::set_tile( const Tile_Map::TILE& tile ){
	m_tile = tile;
	get_ability_data()->get_rendering()->animator.set_frame( m_tile.sprite_index );
}

const Tile_Map::TILE& CTile::get_tile() const{
	return m_tile;
}

void CWall::collision( Game_Object::Ability::CAbility_Data* pdata ){

	if( pdata->get_position() == nullptr || pdata->get_physics() == nullptr ) return;

	Vector2& wall_position = get_ability_data()->get_position()->position;
	Vector2& wall_scale	   = get_ability_data()->get_position()->vscaling;
	Vector2& unit_position = pdata->get_position()->position;
	Vector2& unit_scale    = pdata->get_position()->vscaling;
	if( CGame_Object_Manager::CMediator::intersect_rect( get_ability_data(), pdata ) ){

		Vector2 vsize_delta		= unit_scale * 0.5f + wall_scale * 0.5f;
		Vector2 ptdelta			= unit_position - wall_position;
		Vector2 vcollision_size = { vsize_delta.fx - fabs( ptdelta.fx ), vsize_delta.fy - fabs( ptdelta.fy ) };
		if( vcollision_size.fx < vcollision_size.fy ){

			if( ptdelta.fx < 0.0f ){    //오브젝트가 왼쪽에 있습니다.

				if( m_bleft_check ){
					unit_position.fx = wall_position.fx - ( wall_scale.fx + unit_scale.fx ) * 0.5f;
					pdata->get_physics()->bcollision_right = true;
				}

			}else{					  //오브젝트가 오른쪽에 있습니다.

				if( m_bright_check ){
					unit_position.fx = wall_position.fx + ( wall_scale.fx + unit_scale.fx ) * 0.5f;
					pdata->get_physics()->bcollision_left = true;
				}

			}

		}else{

			if( ptdelta.fy < 0.0f ){    //오브젝트가 위쪽에 있습니다.

				if( m_btop_check ){

					unit_position.fy = wall_position.fy - ( wall_scale.fy + unit_scale.fy ) * 0.5f;
					pdata->get_physics()->bcollision_bottom = true;

					if( Ability::is_jump_down( *pdata ) ){
						Ability::setting_normal( pdata );
						pdata->get_movement()->vdirection = get_direction();
					}

				}

			}else{					  //오브젝트가 아래쪽에 있습니다.
				
				if( m_bbottom_check ){

					unit_position.fy = wall_position.fy + ( wall_scale.fy + unit_scale.fy ) * 0.5f;
					pdata->get_physics()->bcollision_top = true;

					if( Ability::is_jump_up( *pdata ) ){
						Ability::setting_normal( pdata );
						pdata->get_physics()->bjump = true;
					}

				}

			}
		}

	}

	if( INPUT->get_state( VK_RETURN ).bdown ){
		//m_tile_render.render_quad();
	}

}

bool CWall::push_crush(){
	return true;
}

bool CWall::is_left_wall(){
	return true;
}

bool CWall::is_right_wall(){
	return true;
}

bool CWall::is_top_wall(){
	return true;
}

bool CWall::is_down_wall(){
	return true;
}

CCrushable_Wall::CCrushable_Wall() : CWall{}{
}

void CCrushable_Wall::collision( Game_Object::Ability::CAbility_Data* pdata ){

	if( pdata->get_position() == nullptr || pdata->get_physics() == nullptr ) return;

	Vector2& wall_position = get_ability_data()->get_position()->position;
	Vector2& wall_scale	   = get_ability_data()->get_position()->vscaling;
	Vector2& unit_position = pdata->get_position()->position;
	Vector2& unit_scale    = pdata->get_position()->vscaling;
	if( CGame_Object_Manager::CMediator::intersect_rect( get_ability_data(), pdata ) ){

		Vector2 vsize_delta		= unit_scale * 0.5f + wall_scale * 0.5f;
		Vector2 ptdelta			= unit_position - wall_position;
		Vector2 vcollision_size = { vsize_delta.fx - fabs( ptdelta.fx ), vsize_delta.fy - fabs( ptdelta.fy ) };
		if( vcollision_size.fx < vcollision_size.fy ){

			if( ptdelta.fx < 0.0f ){    //오브젝트가 왼쪽에 있습니다.

				unit_position.fx = wall_position.fx - ( wall_scale.fx + unit_scale.fx ) * 0.5f;
				pdata->get_physics()->bcollision_right = true;

			}else{					  //오브젝트가 오른쪽에 있습니다.

				unit_position.fx = wall_position.fx + ( wall_scale.fx + unit_scale.fx ) * 0.5f;
				pdata->get_physics()->bcollision_left = true;

			}

			if( pdata->get_physics()->bcrushable_x ){

				crush_effect();
				reset_check();
				destroy();

				CPlayer::s_iscore += 100;

			}

		}else{

			if( ptdelta.fy < 0.0f ){    //오브젝트가 위쪽에 있습니다.

				unit_position.fy = wall_position.fy - ( wall_scale.fy + unit_scale.fy ) * 0.5f;
				pdata->get_physics()->bcollision_bottom = true;

				if( Ability::is_jump_down( *pdata ) ){
					Ability::setting_normal( pdata );
					pdata->get_movement()->vdirection = get_direction();
				}

			}else{					  //오브젝트가 아래쪽에 있습니다.

				unit_position.fy = wall_position.fy + ( wall_scale.fy + unit_scale.fy ) * 0.5f;
				pdata->get_physics()->bcollision_top = true;

				//이 벽은 아래쪽에서 박치기로 파괴됩니다.
				if( Ability::is_jump_up( *pdata ) ){

					Ability::setting_normal( pdata );
					pdata->get_physics()->bjump = true;

					if( pdata->get_physics()->bcrushable_up ){
						
						crush_effect();
						reset_check();
						destroy();

						CPlayer::s_iscore += 100;

					}

				}

			}

		}

	}

}

bool CCrushable_Wall::push_crush(){

	crush_effect();
	reset_check();
	destroy();

	return true;

}

void CCrushable_Wall::crush_effect(){

	const Vector2& position = get_ability_data()->get_position()->position;
	const Vector2& vscale	= get_ability_data()->get_position()->vscaling;

	std::weak_ptr< Game_Object::CGame_Object > pweak0 = Game_Object::CGame_Object_Manager::CMediator::create_effect< CWall_Effect >( { position.fx - vscale.fx * 0.25f, position.fy -  vscale.fy * 0.25f }, vscale * 0.5f );
	std::weak_ptr< Game_Object::CGame_Object > pweak1 = Game_Object::CGame_Object_Manager::CMediator::create_effect< CWall_Effect >( { position.fx + vscale.fx * 0.25f, position.fy -  vscale.fy * 0.25f }, vscale * 0.5f );
	std::weak_ptr< Game_Object::CGame_Object > pweak2 = Game_Object::CGame_Object_Manager::CMediator::create_effect< CWall_Effect >( { position.fx - vscale.fx * 0.25f, position.fy +  vscale.fy * 0.25f }, vscale * 0.5f );
	std::weak_ptr< Game_Object::CGame_Object > pweak3 = Game_Object::CGame_Object_Manager::CMediator::create_effect< CWall_Effect >( { position.fx + vscale.fx * 0.25f, position.fy +  vscale.fy * 0.25f }, vscale * 0.5f );
	assert( !pweak0.expired() &&
			!pweak1.expired() &&
			!pweak2.expired() &&
			!pweak3.expired() );

	( ( CWall_Effect* )pweak0.lock().get() )->get_ability_data()->get_position()->vdirection = { -1.0f, 0.0f };
	( ( CWall_Effect* )pweak1.lock().get() )->get_ability_data()->get_position()->vdirection = { 1.0f, 0.0f };
	( ( CWall_Effect* )pweak2.lock().get() )->get_ability_data()->get_position()->vdirection = { -1.0f, 0.0f };
	( ( CWall_Effect* )pweak3.lock().get() )->get_ability_data()->get_position()->vdirection = { 1.0f, 0.0f };

	pweak0.lock()->get_ability_data()->get_rendering()->animator.set_frame( get_tile().sprite_index );
	pweak1.lock()->get_ability_data()->get_rendering()->animator.set_frame( get_tile().sprite_index );
	pweak2.lock()->get_ability_data()->get_rendering()->animator.set_frame( get_tile().sprite_index );
	pweak3.lock()->get_ability_data()->get_rendering()->animator.set_frame( get_tile().sprite_index );

	Ability::setting_jump( pweak0.lock()->get_ability_data() );
	Ability::setting_jump( pweak1.lock()->get_ability_data() );
	Ability::setting_small_jump( pweak2.lock()->get_ability_data() );
	Ability::setting_small_jump( pweak3.lock()->get_ability_data() );
	pweak0.lock()->get_ability_data()->get_physics()->faccel_speed *= 3.0f;
	pweak1.lock()->get_ability_data()->get_physics()->faccel_speed *= 3.0f;
	pweak2.lock()->get_ability_data()->get_physics()->faccel_speed *= 3.0f;
	pweak3.lock()->get_ability_data()->get_physics()->faccel_speed *= 3.0f;

	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( pweak0 );
	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( pweak1 );
	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( pweak2 );
	GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( pweak3 );

	CSoundMgr::Get_Instance()->play_sound( TEXT( "Break" ), CSoundMgr::CHANNELID::EFFECT );

}

CItem_Wall::CItem_Wall() : CWall{},
						   m_benable{ true }{
}

void CItem_Wall::collision( Game_Object::Ability::CAbility_Data* pdata ){

	if( pdata->get_position() == nullptr || pdata->get_physics() == nullptr ) return;

	Vector2& wall_position = get_ability_data()->get_position()->position;
	Vector2& wall_scale	   = get_ability_data()->get_position()->vscaling;
	Vector2& unit_position = pdata->get_position()->position;
	Vector2& unit_scale    = pdata->get_position()->vscaling;
	if( CGame_Object_Manager::CMediator::intersect_rect( get_ability_data(), pdata ) ){

		Vector2 vsize_delta		= unit_scale * 0.5f + wall_scale * 0.5f;
		Vector2 ptdelta			= unit_position - wall_position;
		Vector2 vcollision_size = { vsize_delta.fx - fabs( ptdelta.fx ), vsize_delta.fy - fabs( ptdelta.fy ) };
		if( vcollision_size.fx < vcollision_size.fy ){

			if( ptdelta.fx < 0.0f ){    //오브젝트가 왼쪽에 있습니다.

				unit_position.fx = wall_position.fx - ( wall_scale.fx + unit_scale.fx ) * 0.5f;
				pdata->get_physics()->bcollision_right = true;

			}else{					  //오브젝트가 오른쪽에 있습니다.

				unit_position.fx = wall_position.fx + ( wall_scale.fx + unit_scale.fx ) * 0.5f;
				pdata->get_physics()->bcollision_left = true;

			}

			if( pdata->get_physics()->bhit_x ){
				hit();
			}

		}else{

			if( ptdelta.fy < 0.0f ){    //오브젝트가 위쪽에 있습니다.

				unit_position.fy = wall_position.fy - ( wall_scale.fy + unit_scale.fy ) * 0.5f;
				pdata->get_physics()->bcollision_bottom = true;

				if( Ability::is_jump_down( *pdata ) ){
					Ability::setting_normal( pdata );
					pdata->get_movement()->vdirection = get_direction();
				}

			}else{					  //오브젝트가 아래쪽에 있습니다.

				unit_position.fy = wall_position.fy + ( wall_scale.fy + unit_scale.fy ) * 0.5f;
				pdata->get_physics()->bcollision_top = true;

				if( Ability::is_jump_up( *pdata ) ){

					Ability::setting_normal( pdata );
					pdata->get_physics()->bjump = true;

					if( pdata->get_physics()->bhit_up ){
						hit();
					}

				}


			}

		}

	}

}

void CItem_Wall::hit(){

	get_ability_data()->get_rendering()->animator.set_frame( { 71, 0 } );

	if( m_benable ){
		
		if( get_tile().attribute0 == Tile_Map::EAttribute::Item_Coin ){

			Game_Object::CGame_Object_Manager::CMediator::create_effect< CCoin_Effect >( get_ability_data()->get_position()->position, get_ability_data()->get_position()->vscaling );

			++CPlayer::s_icoin;
			CPlayer::s_iscore += 200;

			CSoundMgr::Get_Instance()->play_sound( TEXT( "Coin" ), CSoundMgr::CHANNELID::EFFECT );

		}else if( get_tile().attribute0 == Tile_Map::EAttribute::Item_Mushroom ){
			
			std::weak_ptr< Game_Object::CGame_Object > pweak = Game_Object::CGame_Object_Manager::CMediator::create_item< CMushroom_Item >( get_ability_data()->get_position()->position - Vector2{ 0.0f, 1.0f } * get_ability_data()->get_position()->vscaling.fy,
																																			get_ability_data()->get_position()->vscaling );
			GAME_OBJECT_MANAGER->get_gravity_unit_group()->insert( pweak );
			GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->insert( pweak );

			CSoundMgr::Get_Instance()->play_sound( TEXT( "Appears" ), CSoundMgr::CHANNELID::EFFECT );

		}

		m_benable = false;

	}

}


void CQuad_Up_Wall::collision( Game_Object::Ability::CAbility_Data* pdata ){

	if( pdata->get_position() == nullptr || pdata->get_physics() == nullptr ) return;

	if( CGame_Object_Manager::CMediator::intersect_rect_x( get_ability_data(), pdata ) ){

		LINE wall_line{ { get_ability_data()->get_position()->left(), get_ability_data()->get_position()->top() },
						{ get_ability_data()->get_position()->right(), get_ability_data()->get_position()->top() } };

		constexpr float fepsilon = 5.5f;
		float fdistance = 10000.0f;
		Vector2 vpt_in_line;
		calc_line_upper_point( wall_line, { pdata->get_position()->position.fx, pdata->get_position()->bottom() }, &fdistance, nullptr, &vpt_in_line );
		if( fdistance * fdistance < fepsilon * fepsilon &&
			pdata->get_physics()->bjump &&
			Ability::is_jump_down( *pdata ) ){

			pdata->get_physics()->bcollision_bottom = true;

			pdata->get_position()->position.fy = vpt_in_line.fy + -pdata->get_position()->vscaling.fy * 0.5f;
			Ability::setting_normal( pdata );
			pdata->get_movement()->vdirection = get_direction();

		}

	}

}

void CDiagonal::collision( Ability::CAbility_Data* pdata ){

	if( pdata->get_position() == nullptr || pdata->get_physics() == nullptr ) return;

	if( CGame_Object_Manager::CMediator::intersect_rect_x( get_ability_data(), pdata ) ){

		LINE wall_line = get_diagonal();

		constexpr float fepsilon   = 10.5f;
		float			fdistance  = 10000.0f;
		float			fparameter = 0.0f;
		Vector2 vpt_in_line;
		calc_line_upper_point( wall_line, { pdata->get_position()->position.fx, pdata->get_position()->bottom() }, &fdistance, &fparameter, &vpt_in_line );
		if( fdistance * fdistance < fepsilon * fepsilon &&
			pdata->get_physics()->bjump &&
			Ability::is_jump_down( *pdata ) ){

			pdata->get_physics()->bcollision_bottom = true;

			//if( fparameter >= 0 && fparameter <= 1.0f ){
				pdata->get_position()->position.fy = vpt_in_line.fy + -pdata->get_position()->vscaling.fy * 0.5f;
				Ability::setting_normal( pdata );
				pdata->get_movement()->vdirection = get_direction();
			//}

		}

	}

}

Vector2 CDiagonal::get_direction() const{
	LINE line = get_diagonal();
	return normalize( line.pt1 - line.pt0 );
}

LINE CDiagonal_Right::get_diagonal() const{
	return { get_ability_data()->get_position()->left_bottom(),
			 get_ability_data()->get_position()->right_top() };
}

LINE CDiagonal_Right_Bottom::get_diagonal() const{
	return { get_ability_data()->get_position()->left_bottom(),
			 get_ability_data()->get_position()->right_center() };
}

LINE CDiagonal_Right_Middle::get_diagonal() const{
	return { get_ability_data()->get_position()->left_center(),
			 get_ability_data()->get_position()->right_top() };
}

LINE CDiagonal_Left::get_diagonal() const{
	return { get_ability_data()->get_position()->left_top(),
			 get_ability_data()->get_position()->right_bottom() };
}

LINE CDiagonal_Left_Bottom::get_diagonal() const{
	return { get_ability_data()->get_position()->left_center(),
			 get_ability_data()->get_position()->right_bottom() };
}

LINE CDiagonal_Left_Middle::get_diagonal() const{
	return { get_ability_data()->get_position()->left_top(),
			 get_ability_data()->get_position()->right_center() };
}
