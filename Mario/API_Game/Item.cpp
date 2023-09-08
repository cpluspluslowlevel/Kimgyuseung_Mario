#include "Item.h"

#include "SoundMgr.h"
#include "Graphics.h"
#include "Wall.h"
#include "Player.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

CItem_Base::CItem_Base::CItem_Base() : CUnit{},
									   m_state { EState::Spawn },
									   m_render{},
									   m_spawn_timer{}{

	get_ability_data()->alloc_rendering();
	get_ability_data()->alloc_movement();
	get_ability_data()->alloc_physics();
	get_ability_data()->alloc_tile();

	m_render.set_data( get_ability_data() );
	get_ability_data()->get_rendering()->pbitmap			= BITMAP_MANAGER->find_bitmap( TEXT( "Item" ) );
	get_ability_data()->get_rendering()->color_key			= RGB( 255, 0, 255 );
	get_ability_data()->get_rendering()->fslide_parameter	= 1.0f;
	get_ability_data()->get_rendering()->animator.set_bitmap( get_ability_data()->get_rendering()->pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Item" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 15 );

	Ability::initialize_physics( get_ability_data() );

	Ability::update_tile_index( get_ability_data() );


	m_spawn_timer.reset();


}

Game_Object::CGame_Object::Update_Result CItem_Base::update(){

	switch( m_state ){
	case EState::Spawn:

		update_spawn();
		if( m_spawn_timer.signal( 1000 ) ){
			set_state( EState::Move );
		}

		break;

	case EState::Move:
		update_move();
		break;
	}
	
	Ability::update_tile_index( get_ability_data() );

	if( get_ability_data()->get_position()->position.fy >= 2000.0f ){
		destroy();
	}

	return get_update_result();

}

void CItem_Base::set_state( EState state ){
	m_state = state;
}

CItem_Base::EState CItem_Base::get_state() const{
	return m_state;
}

CMushroom_Item::CMushroom_Item() : CItem_Base{}{

	get_ability_data()->get_position()->vdirection = Vector2{ 1.0f, 0.0f };

	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->fslide_parameter = 0.0f;

	m_movement.set_data( get_ability_data() );
	m_movement.set_speed( 200.0f );

}

void CMushroom_Item::hit_player( Game_Object::Ability::CAbility_Data* pability ){

	if( get_state() == EState::Move ){

		Ability::ITEM_ABILITY_DATA* pitem_ability_data = pability->get_item();
		if( !pitem_ability_data->bacquire ){

			pitem_ability_data->bacquire	= true;
			pitem_ability_data->item		= Ability::EItem::Mushroom;

			CSoundMgr::Get_Instance()->play_sound( TEXT( "Power_Up" ), CSoundMgr::CHANNELID::EFFECT );

			destroy();

		}

	}

}

void CMushroom_Item::update_spawn(){

	get_ability_data()->get_rendering()->fslide_parameter += 1.0f * TIME_MANAGER->get_tick();

	m_render.render_slide();

}

void CMushroom_Item::update_move(){
	m_movement.update();
	m_render.render();
}

CCoin::CCoin(){

	get_ability_data()->get_rendering()->animator.set_bitmap( BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) ) );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Tile" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 56, 4 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 300 );

}

void CCoin::hit_player( Ability::CAbility_Data* pability ){

	Ability::ITEM_ABILITY_DATA* pitem_ability_data = pability->get_item();
	if( !pitem_ability_data->bacquire ){

		pitem_ability_data->bacquire	= true;
		pitem_ability_data->item		= Ability::EItem::Coin;

		++CPlayer::s_icoin;
		CPlayer::s_iscore += 200;

		CSoundMgr::Get_Instance()->play_sound( TEXT( "Coin" ), CSoundMgr::CHANNELID::EFFECT );

		destroy();

	}

}

void CCoin::update_spawn(){
	m_render.render();
}

void CCoin::update_move(){
	m_render.render();
}


CGoal_Item::CGoal_Item() : CItem_Base{}{

	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Item_Goal" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 300 );

}

void CGoal_Item::hit_player( Game_Object::Ability::CAbility_Data* pability ){

	Ability::ITEM_ABILITY_DATA* pitem_ability_data = pability->get_item();
	if( !pitem_ability_data->bacquire ){

		pitem_ability_data->bacquire	= true;
		pitem_ability_data->item		= Ability::EItem::Clear;

		for( int i = 0; i< 3; ++i ){
			if( CPlayer::s_iclear_item[ i ] == 0 ){
				CPlayer::s_iclear_item[ i ] = get_ability_data()->get_rendering()->animator.get_frame().iy + 1;
				break;
			}
		}

		destroy();

	}

}

void CGoal_Item::update_spawn(){
	get_ability_data()->get_rendering()->animator.update_y();
	m_render.render();
}

void CGoal_Item::update_move(){
	get_ability_data()->get_rendering()->animator.update_y();
	m_render.render();
}