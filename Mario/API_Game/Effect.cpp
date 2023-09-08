#include "Effect.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

CEffect_Base::CEffect_Base() : CUnit{}{

	get_ability_data()->alloc_rendering();

	m_render.set_data( get_ability_data() );
	

}

Game_Object::CGame_Object::Update_Result CEffect_Base::update(){
	m_render.render();
	return get_update_result();
}

CCoin_Effect::CCoin_Effect() : CEffect_Base{},
							   m_binverse_animation{ false }{

	get_ability_data()->get_rendering()->pbitmap	= BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) );
	get_ability_data()->get_rendering()->color_key	= RGB( 255, 0, 255 );
	get_ability_data()->get_rendering()->animator.set_bitmap( get_ability_data()->get_rendering()->pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Coin_Effect" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 15 );

}

Game_Object::CGame_Object::Update_Result CCoin_Effect::update(){

	if( m_binverse_animation ){

		get_ability_data()->get_rendering()->animator.inverse_update();
		if( get_ability_data()->get_rendering()->animator.get_frame().ix  == 56  ){
			destroy();
		}
		
	}else{

		get_ability_data()->get_rendering()->animator.update();
		if( get_ability_data()->get_rendering()->animator.get_frame().ix  == 60  ){
			m_binverse_animation = true;
		}

	}

	


	get_ability_data()->get_position()->position.fy -= 1000.0f * TIME_MANAGER->get_tick();

	return CEffect_Base::update();

}

CWall_Effect::CWall_Effect() : CEffect_Base{},
							   m_movement{}{

	get_ability_data()->alloc_movement();
	get_ability_data()->alloc_physics();

	get_ability_data()->get_rendering()->pbitmap	= BITMAP_MANAGER->find_bitmap( TEXT( "Tile" ) );
	get_ability_data()->get_rendering()->color_key	= RGB( 255, 0, 255 );
	get_ability_data()->get_rendering()->animator.set_bitmap( get_ability_data()->get_rendering()->pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Tile" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 61, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 15 );

	m_movement.set_data( get_ability_data() );
	get_ability_data()->get_movement()->fspeed			   =  150.0f;
	get_ability_data()->get_movement()->vdirection		   =  { 1.0f, 0.0f };
	get_ability_data()->get_movement()->fsliding_speed	   =  0.0f;
	get_ability_data()->get_movement()->fsliding_max_speed =  800.0f;
	get_ability_data()->get_movement()->fsliding_accel     =  600.0f;

	Ability::initialize_physics( get_ability_data() );

}

Game_Object::CGame_Object::Update_Result CWall_Effect::update(){
	m_movement.move_front();
	return CEffect_Base::update();
}