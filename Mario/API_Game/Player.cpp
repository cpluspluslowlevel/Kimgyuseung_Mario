#include "Player.h"

#include "SoundMgr.h"
#include "Graphics.h"
#include "Time.h"
#include "Input.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING;

int CPlayer::s_icoin			= 0;
int CPlayer::s_ilife			= 5;
int CPlayer::s_iscore			= 0;
int CPlayer::s_iclear_item[ 3 ] = { 0, 0, 0 };

CPlayer::CPlayer() : m_render{},
					 m_movement{},
					 m_physics{},
					 m_mode{ EMode::Small },
					 m_old_mode{ EMode::Small },
					 m_sprite{ ESprite::Stand },
					 m_old_sprite{ ESprite::Stand },
					 m_sprite_set{},
					 m_bleft{ false },
					 m_bold_left{ false },
					 m_control_mode{ EControl_Mode::Normal },
					 m_old_control_mode{ EControl_Mode::Normal },
					 m_clear_timer{},
					 m_iaccel{ 0 },
					 m_accel_timer{},
					 m_bjump_pressed{ false },
					 m_pressed_jump_timer{},
					 m_fmax_jump_speed{ 0.0f },
					 m_dwpipe_stage{ 0 },
					 m_dwpipe_position{ 0 },
					 m_die_move_timer{},
					 m_die_scene_move_timer{},
					 m_no_damage_timer{},
					 m_no_damage_render_timer{},
					 m_bno_damage{ false },
					 m_bno_damage_render_on{ true }{

	initialize_resource();

	get_ability_data()->alloc_rendering();
	get_ability_data()->alloc_movement();
	get_ability_data()->alloc_physics();
	get_ability_data()->alloc_tile();
	get_ability_data()->alloc_item();
	get_ability_data()->alloc_interaction();
	get_ability_data()->alloc_fight();

	get_position()->set_position( 400, 300 );

	m_render.set_data( get_ability_data() );
	get_ability_data()->get_rendering()->color_key			= RGB( 255, 0, 255 );
	get_ability_data()->get_rendering()->fslide_parameter	= 1.0f;

	m_movement.set_data( get_ability_data() );
	get_ability_data()->get_movement()->fspeed			   =  400.0f;
	get_ability_data()->get_movement()->vdirection		   =  { 1.0f, 0.0f };
	get_ability_data()->get_movement()->fsliding_speed	   =  0.0f;
	get_ability_data()->get_movement()->fsliding_max_speed =  800.0f;
	get_ability_data()->get_movement()->fsliding_accel     =  600.0f;

	m_physics.set_data( get_ability_data() );
	Ability::initialize_physics( get_ability_data() );

	Ability::initialize_interaction( get_ability_data() );

	Ability::initialize_fight( get_ability_data() );

	Ability::update_tile_index( get_ability_data() );
	change_control_mode();
	change_mode();

	m_no_damage_timer.reset();

}

DWORD CPlayer::update(){

	m_physics.save_data();

	switch( m_control_mode ){
	case EControl_Mode::Normal:
		control_normal();
		break;
	case EControl_Mode::Item:
		control_item();
		break;
	case EControl_Mode::Clear:
		control_clear();
		break;
	case EControl_Mode::Pipe:
		control_pipe();
		break;
	case EControl_Mode::Die:
		control_die();
		break;
	}

	CGame_Object_Manager::CMediator::get_scroll()->set_pt( get_position()->get_position() );

	//방향에 따라 스프라이트를 맞춰줍니다.
	if( m_bleft ){
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_origin();
	}else{
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_flip();
	}

	//방향이 바뀌었으면 가속을 초기화 합니다.
	if( m_bleft != m_bold_left ){
		m_iaccel = 0;
	}

	m_bold_left = m_bleft;

	if( m_bno_damage ){

		if( m_no_damage_timer.signal( 1000 ) ){
			m_bno_damage = false;
		}

	}

	get_ability_data()->get_rendering()->animator.update();
	if( !m_bno_damage || m_bno_damage_render_on ){

		switch( m_control_mode ){
		case EControl_Mode::Pipe:
			m_render.render_slide();
			break;
		default:
			m_render.render();
			break;
		}


	}

	if( m_no_damage_render_timer.signal( 100 ) ){
		m_bno_damage_render_on = !m_bno_damage_render_on;
	}

	if( INPUT->get_state( '1' ).bpush ){
		set_mode( EMode::Small );
	}else if( INPUT->get_state( '2' ).bpush ){
		set_mode( EMode::Big );
	}

	return get_update_result();

}

void CPlayer::update_late(){

	switch( calc_accel_level() ){
	case 0:
		get_ability_data()->get_movement()->fspeed = 400.0f;
		break;
	case 1:
		get_ability_data()->get_movement()->fspeed = 600.0f;
		break;
	case 2:
		get_ability_data()->get_movement()->fspeed = 800.0f;
		break;
	default:
		assert( false );
	}

	Ability::update_tile_index( get_ability_data() );

}

void CPlayer::clear_koopa(){
	set_update_result( UPDATE_RESULT_PLAYER_CLEAR );
}

void CPlayer::damage(){

	if( !m_bno_damage ){
		switch( m_mode ){
		case EMode::Small:
			set_controll_mode( EControl_Mode::Die );
			break;
		case EMode::Big:
			set_mode( EMode::Small );
			m_bno_damage = true;
			m_no_damage_timer.reset();
			break;
		}
	}

}

void CPlayer::set_mode( EMode mode ){
	if( m_mode != mode ){
		m_old_mode = m_mode;
		m_mode	   = mode;
		change_mode();
	}
}

void CPlayer::set_sprite( ESprite state ){
	if( m_sprite != state ){
		m_old_sprite = m_sprite;
		m_sprite	 = state;
		change_sprite();
	}
}

void CPlayer::set_controll_mode( EControl_Mode mode ){
	if( m_control_mode != mode){
		m_old_control_mode = m_control_mode;
		m_control_mode	   = mode;
		change_control_mode();
	}
}

void CPlayer::set_pipe_stage( DWORD dwstage  ){
	m_dwpipe_stage = dwstage;
}

void CPlayer::set_pipe_position( DWORD dwposition ){
	m_dwpipe_position = dwposition;
}

int CPlayer::get_accel() const{
	return m_iaccel;
}

DWORD CPlayer::get_pipe_stage() const{
	return m_dwpipe_stage;
}

DWORD CPlayer::get_pipe_position() const{
	return m_dwpipe_position;
}

void CPlayer::initialize_resource(){

	m_sprite_set[ ( size_t )EMode::Small ].pbitmap				= BITMAP_MANAGER->find_bitmap( TEXT( "Mario_Small" ) );
	m_sprite_set[ ( size_t )EMode::Small ].pstand_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Stand" ) );
	m_sprite_set[ ( size_t )EMode::Small ].pwalk_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Walk" ) );
	m_sprite_set[ ( size_t )EMode::Small ].prun_max_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Run" ) );
	m_sprite_set[ ( size_t )EMode::Small ].pjump_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Jump" ) );
	m_sprite_set[ ( size_t )EMode::Small ].ppower_jump_sprite	= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Power_Jump" ) );
	m_sprite_set[ ( size_t )EMode::Small ].psliding_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Sliding" ) );
	m_sprite_set[ ( size_t )EMode::Small ].pdie_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Small_Die" ) );
	m_sprite_set[ ( size_t )EMode::Small ].vsize				= { 50.0f, 50.0f };

	m_sprite_set[ ( size_t )EMode::Big ].pbitmap				= BITMAP_MANAGER->find_bitmap( TEXT( "Mario_Big" ) );
	m_sprite_set[ ( size_t )EMode::Big ].pstand_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Stand" ) );
	m_sprite_set[ ( size_t )EMode::Big ].pwalk_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Walk" ) );
	m_sprite_set[ ( size_t )EMode::Big ].prun_max_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Run" ) );
	m_sprite_set[ ( size_t )EMode::Big ].pjump_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Jump" ) );
	m_sprite_set[ ( size_t )EMode::Big ].ppower_jump_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Power_Jump" ) );
	m_sprite_set[ ( size_t )EMode::Big ].psliding_sprite		= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Sliding" ) );
	m_sprite_set[ ( size_t )EMode::Big ].pdie_sprite			= BITMAP_MANAGER->find_sprite( TEXT( "Mario_Big_Sliding" ) );
	m_sprite_set[ ( size_t )EMode::Big ].vsize					= { 50.0f, 100.0f };

}

void CPlayer::change_mode(){

	switch( m_mode ){
	case EMode::Small:
		get_position()->set_scale( m_sprite_set[ ( size_t )m_mode ].vsize );
		get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set[ ( size_t )m_mode ].pbitmap );
		get_ability_data()->get_physics()->bcrushable_x		= false;
		get_ability_data()->get_physics()->bcrushable_up	= false;
		get_ability_data()->get_physics()->bhit_x			= false;
		get_ability_data()->get_physics()->bhit_up			= true;
		break;
	case EMode::Big:
		get_position()->set_scale( m_sprite_set[ ( size_t )m_mode ].vsize );
		get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set[ ( size_t )m_mode ].pbitmap );
		get_ability_data()->get_physics()->bcrushable_x		= false;
		get_ability_data()->get_physics()->bcrushable_up	= true;
		get_ability_data()->get_physics()->bhit_x			= false;
		get_ability_data()->get_physics()->bhit_up			= true;
		break;
	}

	change_sprite();

}

void CPlayer::change_sprite(){

	switch( m_sprite ){
	case ESprite::Stand:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].pstand_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 100 );
		break;
	case ESprite::Walk:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].pwalk_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 150 );
		break;
	case ESprite::Run:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].pwalk_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 25 );
		break;
	case ESprite::Run_Max:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].prun_max_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 25 );
		break;
	case ESprite::Jump:
		if( m_old_sprite == ESprite::Run_Max ){
			get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].ppower_jump_sprite );
		}else{
			get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].pjump_sprite );
		}
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 100 );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		break;
	case ESprite::Sliding:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].psliding_sprite );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 100 );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		break;
	case ESprite::Die:
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set[ ( size_t )m_mode ].pdie_sprite );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 100 );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		break;
	}

}

void CPlayer::change_control_mode(){

	switch( m_control_mode ){
	case EControl_Mode::Normal:
		break;
	case EControl_Mode::Item:
		break;
	case EControl_Mode::Clear:

		m_clear_timer.reset();
		m_iaccel = 0;

		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->play_sound( TEXT( "Clear" ), CSoundMgr::CHANNELID::EFFECT );

		break;

	case EControl_Mode::Pipe:

		set_sprite( ESprite::Stand );
		get_ability_data()->get_rendering()->fslide_parameter = 1.0f;

		CSoundMgr::Get_Instance()->play_sound( TEXT( "Pipe" ), CSoundMgr::CHANNELID::EFFECT );

		break;

	case EControl_Mode::Die:
		
		set_sprite( ESprite::Die );

		m_die_move_timer.reset();
		m_die_scene_move_timer.reset();

		GAME_OBJECT_MANAGER->get_gravity_unit_group()->delete_ptr( get_this() );
		GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->delete_ptr( get_this() );

		--s_ilife;

		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->play_sound( TEXT( "Lost" ), CSoundMgr::CHANNELID::EFFECT );

		break;

	}

}

void CPlayer::input_move( DWORD dwleft_key, DWORD dwright_key, DWORD dwaccel_key, DWORD dwsliding_key ){

	bool bsliding = false;

	if( INPUT->get_state( dwleft_key ).bdown ){
		m_movement.move_left();
		change_state_move();
		m_bleft = true;
	}else if( INPUT->get_state( dwright_key ).bdown ){
		m_movement.move_right();
		change_state_move();
		m_bleft = false;
	}else if( !get_ability_data()->get_physics()->bjump ){
		
		if(	INPUT->get_state( dwsliding_key ).bdown ){
			bsliding = m_movement.sliding();
		}else{
			set_sprite( ESprite::Stand );
		}


	}

	if( bsliding ){

		set_sprite( ESprite::Sliding );
		get_ability_data()->get_fight()->benable_attack = true;
		get_ability_data()->get_fight()->benable_hit	= false;

	}else{
		
		get_ability_data()->get_movement()->fsliding_speed	   =  0.0f;
		get_ability_data()->get_movement()->fsliding_max_speed =  800.0f;
		get_ability_data()->get_movement()->fsliding_accel     =  1000.0f;

		get_ability_data()->get_fight()->benable_attack = false;
		get_ability_data()->get_fight()->benable_hit	= true;

	}

	if( INPUT->get_state( dwaccel_key ).bpush ){
		m_accel_timer.reset();
	}
	if( INPUT->get_state( dwaccel_key ).bup ){
		m_accel_timer.reset();
	}

	if( m_accel_timer.signal( 150 ) ){

		if( INPUT->get_state( dwaccel_key ).bdown ){
			m_iaccel = min( m_iaccel + 1, 10 );
		}else{
			m_iaccel = max( m_iaccel - 1, 0 );
		}

	}

}

void CPlayer::input_jump( DWORD dwkey ){

	if( INPUT->get_state( dwkey ).bdown ){
	
		//이미 점프중 점프 속도를 증가시키고 아니면 점프를 시작합니다.
		if( get_ability_data()->get_physics()->bjump ){

			if( Ability::is_jump_up( *get_ability_data() ) ){

				get_ability_data()->get_physics()->fjump_speed -= 500.0f * TIME_MANAGER->get_tick();
				if( get_ability_data()->get_physics()->fjump_speed < m_fmax_jump_speed ){
					get_ability_data()->get_physics()->fjump_speed = m_fmax_jump_speed;
				}

			}

		}else{

			jump();
			m_bjump_pressed = true;
			m_pressed_jump_timer.reset();
			set_sprite( ESprite::Jump );
			
			CSoundMgr::Get_Instance()->play_sound( TEXT( "Jump" ), CSoundMgr::CHANNELID::EFFECT );

		}

	}else{
		m_bjump_pressed = false;
	}

}

void CPlayer::control_normal(){

	check_item();

	input_move( VK_LEFT, VK_RIGHT, 'Z', VK_DOWN );
	input_jump( 'C' );

	//점프중 내려오면서 적들을 밟았을 경우 소점프 합니다.
	if( get_ability_data()->get_interaction()->bpush &&
		Ability::is_jump_down( *get_ability_data() ) ){
		Ability::setting_small_jump( get_ability_data() );
	}

	//화면 밑으로 떨어지면 사망합니다.
	if( get_ability_data()->get_position()->position.fy > CGame_Object_Manager::CMediator::get_scroll()->get_pt().fy + 600.0f + 100.0f ){
		damage();
	}

}

void CPlayer::control_item(){
}

void CPlayer::control_clear(){

	if( !get_ability_data()->get_physics()->bjump ){

		m_movement.move_right();
		change_state_move();
		m_bleft = false;

	}

	if( m_clear_timer.signal( 5000 ) ){
		set_update_result( UPDATE_RESULT_PLAYER_CLEAR );
	}

}

void CPlayer::control_pipe(){

	get_ability_data()->get_rendering()->fslide_parameter -= 1.0f * TIME_MANAGER->get_tick();
	if( get_ability_data()->get_rendering()->fslide_parameter <= 0.0f ){
		set_update_result(  UPDATE_RESULT_PLAYER_PIPE );
	}

}

void CPlayer::control_die(){

	//일정 시간 멈춰있다가 떨어집니다.
	if( m_die_move_timer.signal( 1000, false ) ){
		get_ability_data()->get_position()->position.fy += 500.0f * TIME_MANAGER->get_tick();
	}

	//일정 시간이 지나면 재시작 합니다.
	if( m_die_scene_move_timer.signal( 3800 ) ){
		set_update_result( UPDATE_RESULT_PLAYER_DIE );
	}

}

void CPlayer::change_state_move(){

	if( !get_ability_data()->get_physics()->bjump ){
		switch( calc_accel_level() ){
		case 0:
			set_sprite( ESprite::Walk );
			break;
		case 1:
			set_sprite( ESprite::Run );
			break;
		case 2:
			set_sprite( ESprite::Run_Max );
			break;
		}
	}

}

void CPlayer::jump(){

	switch( calc_accel_level() ){
	case 0:
		Ability::setting_jump( get_ability_data() );
		m_fmax_jump_speed = -700.0f;
		break;
	case 1:
		Ability::setting_power_jump( get_ability_data() );
		m_fmax_jump_speed = -800.0f;

		break;
	case 2:
		Ability::setting_max_jump( get_ability_data() );
		m_fmax_jump_speed = -1000.0f;
		break;
	}

}

void CPlayer::check_item(){

	ESprite state = m_sprite;

	if( get_ability_data()->get_item()->bacquire ){

		switch( get_ability_data()->get_item()->item ){
		case Ability::EItem::Mushroom:
			set_mode( EMode::Big );
			break;

		case Ability::EItem::Flower:
			break;
		case Ability::EItem::Star:
			break;
		case Ability::EItem::Clear:
			set_controll_mode( EControl_Mode::Clear );
			break;
		}

	}

}

int CPlayer::calc_accel_level() const{

	if( m_iaccel < 5 ){
		return 0;
	}else if( m_iaccel < 10 ){
		return 1;
	}else if( m_iaccel == 10 ){
		return 2;
	}

	return 0;

}
