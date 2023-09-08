#include "Enemy.h"

#include "SoundMgr.h"
#include "Graphics.h"
#include "Tile.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING

CEnemy_Base::CEnemy_Base() : CUnit{},
						     m_render {},
							 m_movement{},
							 m_sprite_set{}{

    get_ability_data()->alloc_rendering();
	get_ability_data()->alloc_movement();
	get_ability_data()->alloc_physics();
	get_ability_data()->alloc_tile();
	get_ability_data()->alloc_interaction();
	get_ability_data()->alloc_fight();

	m_render.set_data( get_ability_data() );
	get_ability_data()->get_rendering()->color_key = RGB( 255, 0, 255 );

	m_movement.set_data( get_ability_data() );
	get_ability_data()->get_movement()->fspeed = 200.0f;
	get_ability_data()->get_movement()->fsliding_speed	   =  0.0f;
	get_ability_data()->get_movement()->vdirection		   =  { 1.0f, 0.0f };
	get_ability_data()->get_movement()->fsliding_max_speed =  800.0f;
	get_ability_data()->get_movement()->fsliding_accel     =  600.0f;
	m_movement.set_state( Ability::CAI_Movement::EState::Y_Check_Normal );

	Ability::initialize_physics( get_ability_data() );

	Ability::initialize_interaction( get_ability_data() );

	Ability::initialize_fight( get_ability_data() );

	Ability::update_tile_index( get_ability_data() );

}

Game_Object::CGame_Object::Update_Result CEnemy_Base::update(){

	m_render.render();

	Ability::update_tile_index( get_ability_data() );

    return get_update_result();

}

void CEnemy_Base::initialize(){

	get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pwalk_sprite );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( m_sprite_set.dwanimation_speed_ms );

}

void CEnemy_Base::set_sprite_set( const SPRITE_SET& sprite_set ){
	m_sprite_set = sprite_set;
}

CGoomba::CGoomba() : CEnemy_Base{},
					 m_state{ EState::Walk }{

	get_ability_data()->get_position()->vscaling = { 40.0f, 40.0f };

	get_ability_data()->get_movement()->fspeed = 200.0f;

}

Game_Object::CGame_Object::Update_Result CGoomba::update(){

	//�ȴ����� ���� ó�� �Դϴ�.
	if( m_state == EState::Walk ){

		//�������� ó���մϴ�.
		if( get_ability_data()->get_interaction()->bpush ){
			push();
		}

		//�΋H���� ���� ó�� �Դϴ�.
		if( get_ability_data()->get_interaction()->bhit_left ||
			get_ability_data()->get_interaction()->bhit_right ){

			CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
			if( pplayer != nullptr ){
				pplayer->damage();
			}

		}

		if( get_ability_data()->get_fight()->bdamage ){
			die_damage();
		}

		m_movement.update();

		get_ability_data()->get_rendering()->animator.update();

	}else if( m_state == EState::Die_Push ){
	
		//������� ���� ó�� �Դϴ�.
		if( get_ability_data()->get_rendering()->animator.get_frame().ix == 3 ){
			destroy();
		}

		get_ability_data()->get_rendering()->animator.update();

	}else if( m_state == EState::Die_Damage ){
	}

	if( get_ability_data()->get_position()->position.fy >= 2000.0f ){
		destroy();
	}

	return CEnemy_Base::update();

}

void CGoomba::push(){

	m_state = EState::Die_Push;
	get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pdie_push_sprite );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 1000 );

	get_ability_data()->get_fight()->benable_hit = false;
	
	CPlayer::s_iscore += 500;

	CSoundMgr::Get_Instance()->play_sound( TEXT( "Stomp" ), CSoundMgr::CHANNELID::EFFECT );

}

void CGoomba::die_damage(){

	m_state = EState::Die_Damage;
	get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pdie_damage_sprite );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 1000 );

	Ability::setting_small_jump( get_ability_data() );

	get_ability_data()->get_fight()->benable_hit = false;

	GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->delete_ptr( get_this() );

	CPlayer::s_iscore += 700;

	CSoundMgr::Get_Instance()->play_sound( TEXT( "Kick" ), CSoundMgr::CHANNELID::EFFECT );

}

CTurtle::CTurtle() : CEnemy_Base{},
				     m_state{ EState::Walk },
					 m_old_movement_state{ Game_Object::Ability::CAI_Movement::EState::Y_Check_Normal }{

	get_ability_data()->get_position()->vscaling = { 50.0f, 80.0f };

	get_ability_data()->get_movement()->fspeed = 150.0f;


}

Game_Object::CGame_Object::Update_Result CTurtle::update(){

	//�ȴ����� ���� ó�� �Դϴ�.
	if( m_state == EState::Walk ){

		//������ �� ó���մϴ�.
		if( get_ability_data()->get_interaction()->bpush ){
			push();
		}

		//�΋H���� ���� ó�� �Դϴ�.
		if( get_ability_data()->get_interaction()->bhit_left ||
			get_ability_data()->get_interaction()->bhit_right ){

			CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
			if( pplayer != nullptr ){
				pplayer->damage();
			}

		}

		check_damage();

		m_movement.update();

		get_ability_data()->get_rendering()->animator.update();

	}else if( m_state == EState::Stun ){		//��� ������ ���� ó�� �Դϴ�.

		if( get_ability_data()->get_interaction()->bhit_left ||
			get_ability_data()->get_interaction()->bhit_right ||
			get_ability_data()->get_interaction()->bpush ){

			m_state = EState::Stun_Walk;

			//�浹�� ������ x�� ���̸� �̿��� ������ �����մϴ�.
			if( get_ability_data()->get_position()->position.fx > get_ability_data()->get_interaction()->pgame_object.lock()->get_ability_data()->get_position()->position.fx ){
				m_movement.set_right();
			}else{
				m_movement.set_left();
			}

			get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 50 );

			get_ability_data()->get_movement()->fspeed = 500.0f;
			m_old_movement_state = m_movement.get_state();
			m_movement.set_state( Game_Object::Ability::CAI_Movement::EState::Normal );

			get_ability_data()->get_physics()->bcrushable_x		= true;
			get_ability_data()->get_physics()->bcrushable_up	= false;
			get_ability_data()->get_physics()->bhit_x			= true;
			get_ability_data()->get_physics()->bhit_up			= false;

			get_ability_data()->get_fight()->benable_attack = true;

			GAME_OBJECT_MANAGER->get_attack_group()->insert( get_this() );

			CSoundMgr::Get_Instance()->play_sound( TEXT( "Kick" ), CSoundMgr::CHANNELID::EFFECT );


		}

		check_damage();

	}else if( m_state == EState::Stun_Walk ){

		//������ �� ó���մϴ�.
		if(	get_ability_data()->get_interaction()->bpush ){
			push();
		}

		//�΋H���� ���� ó�� �Դϴ�.
		if( ( get_ability_data()->get_interaction()->bhit_left && m_movement.is_left_move() ) ||
			( get_ability_data()->get_interaction()->bhit_right && m_movement.is_right_move() ) ){

			CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
			if( pplayer != nullptr ){
				pplayer->damage();
			}

		}

		check_damage();

		m_movement.update();

		get_ability_data()->get_rendering()->animator.update();

	}else if( m_state == EState::Die_Damage ){
	}

	

	if( get_ability_data()->get_position()->position.fy >= 2000.0f ){
		destroy();
	}

	if( m_movement.is_left_move() ){
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_origin();
	}else{
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_flip();
	}

	return CEnemy_Base::update();

}

void CTurtle::push(){
	
	get_ability_data()->get_position()->vscaling = { 40.0f, 40.0f };

	m_state = EState::Stun;
	get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pstun_sprite );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 1000 );

	m_movement.set_state( m_old_movement_state );

	get_ability_data()->get_fight()->benable_attack = false;

	GAME_OBJECT_MANAGER->get_attack_group()->delete_ptr( get_this() );

	CSoundMgr::Get_Instance()->play_sound( TEXT( "Stomp" ), CSoundMgr::CHANNELID::EFFECT );

}

void CTurtle::die_damage(){

	m_state = EState::Die_Damage;

	get_ability_data()->get_position()->vscaling = { 40.0f, 40.0f };

	get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pdie_damage_sprite );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 1000 );

	Ability::setting_small_jump( get_ability_data() );

	get_ability_data()->get_fight()->benable_hit = false;

	GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->delete_ptr( get_this() );

	CPlayer::s_iscore += 1500;

	CSoundMgr::Get_Instance()->play_sound( TEXT( "Kick" ), CSoundMgr::CHANNELID::EFFECT );

}

void CTurtle::check_damage(){

	//�¾����� ó���մϴ�.
	if( get_ability_data()->get_fight()->bdamage &&
		!get_ability_data()->get_fight()->pgame_object.expired() &&
		get_ability_data()->get_fight()->pgame_object.lock().get() != this ){
		die_damage();
	}

}

CKoopa::CKoopa() : CEnemy_Base{},
				   m_state{ EState::Fire },
				   m_ifire_count{ 0 },
				   m_ptjump_target{},
				   m_push_stop_timer{},
				   m_bpush_old_jump{ false },
				   m_bpush_crush_wall{ false },
				   m_die_timer{},
				   m_fdie_quake{ 0.0f },
				   m_fdie_quake_direction{ 1.0f }{

	get_ability_data()->get_position()->vscaling = { 100.0f, 120.0f };

	get_ability_data()->get_movement()->fspeed = 150.0f;

	get_ability_data()->get_interaction()->benable_push = false;

	set_state( EState::Spawn );


}

Game_Object::CGame_Object::Update_Result CKoopa::update(){

	switch( m_state ){
	case EState::Spawn:
		spawn();
		break;
	case EState::Fire:
		fire();
		break;
	case EState::Jump:
		jump();
		break;
	case EState::Push:
		push();
		break;
	case EState::Die:
		die();
		break;
	}

	//�΋H���� ���� ó�� �Դϴ�.
	if( get_ability_data()->get_interaction()->bhit_left ||
		get_ability_data()->get_interaction()->bhit_right ||
		get_ability_data()->get_interaction()->bhit_top ||
		get_ability_data()->get_interaction()->bhit_bottom ){

		CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
		if( pplayer != nullptr ){
			pplayer->damage();
		}

	}

	return CEnemy_Base::update();

}

void CKoopa::set_state( EState state ){

	if( m_state != state ){
		m_state = state;
		change_state();
	}

}

void CKoopa::change_state(){

	switch( m_state ){
	case EState::Fire:

		get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pfire_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 500 );

		//���� �Ŀ� �� �������� ���ƿ��Ƿ� ������ �ӵ��� �����մϴ�.
		get_ability_data()->get_physics()->faccel_speed /= 3.0f;

		m_ifire_count = 0;

		break;
	case EState::Jump:

		get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.pjump_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 1000 );

		break;
	case EState::Push:

		get_ability_data()->get_rendering()->animator.set_bitmap( m_sprite_set.pbitmap );
		get_ability_data()->get_rendering()->animator.set_sprite( m_sprite_set.ppush_sprite );
		get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
		get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 500 );

		Ability::setting_normal( get_ability_data() );
		get_ability_data()->get_physics()->faccel_speed *= 3.0f;

		m_push_stop_timer.reset();
		m_bpush_old_jump	= false;
		m_bpush_crush_wall	= true;

		break;

	case EState::Die:

		m_die_timer.reset();
		m_fdie_quake			= 50.0f;
		m_fdie_quake_direction	= 1.0f;

		break;

	}

}

void CKoopa::setting_direction(){

	CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
	if( pplayer != nullptr ){

		if( pplayer->get_ability_data()->get_position()->position.fx < get_ability_data()->get_position()->position.fx ){
			( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_origin();
		}else{
			( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_flip();
		}

	}

}

void CKoopa::crush_wall(){

	Tile_Map::TILE_INDEX tile_index = TILE_MAP->calc_tile_index( get_ability_data()->get_position()->position );

	//���� Ÿ�� �翷���� ������ �������鼭 Ž���մϴ�.
	Tile_Map::TILE_INDEX begin_index{ tile_index.ix - 1, tile_index.iy };
	begin_index.ix = max( 0, min( TILE_MAP->get_tile_map_property().tile_map_size.ix - 1, begin_index.ix ) );
	begin_index.iy = max( 0, min( TILE_MAP->get_tile_map_property().tile_map_size.iy - 1, begin_index.iy ) );

	Tile_Map::TILE_INDEX end_index{ tile_index.ix + 1, TILE_MAP->get_tile_map_property().tile_map_size.iy - 1 };
	end_index.ix = max( 0, min( TILE_MAP->get_tile_map_property().tile_map_size.ix - 1, end_index.ix ) );
	end_index.iy = max( 0, min( TILE_MAP->get_tile_map_property().tile_map_size.iy - 1, end_index.iy ) );

	bool bcrush = false;
	for( long long iy = begin_index.iy; iy <= end_index.iy; ++iy ){
		for( long long ix = begin_index.ix; ix <= end_index.ix; ++ix ){

			std::weak_ptr< CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( { ix, iy } );
			if( !pweak_tile.expired() ){

				auto ptile = pweak_tile.lock();
				if( ( ( CTile* )ptile.get() )->push_crush() ){
					bcrush = true;
				}

			}

		}

		//�̹� ���ο��� �ı��Ѱ� ������ Ž���� �����մϴ�.
		if( bcrush ){
			break;
		}


	}

}

void CKoopa::spawn(){

	if( !get_ability_data()->get_physics()->bjump ){
		set_state( EState::Fire );
	}

	SetWindowText( GRAPHICS_MANAGER->get_window_handle(), TEXT( "Spawn" ) );

}

void CKoopa::fire(){

	int iold_frame_x = get_ability_data()->get_rendering()->animator.get_frame().ix;

	get_ability_data()->get_rendering()->animator.update();

	//�÷��̾ �ٶ󺾴ϴ�.
	//�׻� �ٶ󺸴°� �ƴϰ� �̻��� �߻� �غ����϶��� �ٶ󺾴ϴ�.
	if( get_ability_data()->get_rendering()->animator.get_frame().ix == 0 ){
		setting_direction();
	}

	//�� �߻� ����� �� ó�� �մϴ�.
	if( iold_frame_x != 2 && get_ability_data()->get_rendering()->animator.get_frame().ix == 2 ){

		CKoopa_Fire* pfire = new CKoopa_Fire{};
		pfire->get_position()->set_position( get_ability_data()->get_position()->position );
		pfire->get_position()->set_scale( { 50.0f, 50.0f } );

		CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
		if( pplayer != nullptr ){
			pfire->get_ability_data()->get_position()->vdirection = normalize( pplayer->get_ability_data()->get_position()->position - get_ability_data()->get_position()->position );
		}else{
			pfire->get_ability_data()->get_position()->vdirection = { -1.0f, 0.0f };
		}

		GAME_OBJECT_MANAGER->insert( pfire, CGame_Object_Manager::EList_Type::Unit );

		//GAME_OBJECT_MANAGER->get_wall_collision_unit_group()->insert( pfire->get_this() );
		GAME_OBJECT_MANAGER->get_push_group()->insert( pfire->get_this() );

		CSoundMgr::Get_Instance()->play_sound( TEXT( "Fire" ), CSoundMgr::CHANNELID::EFFECT );

		++m_ifire_count;

	}

	//��� ó������ ���� ������ ���� ����� �߻� ������ ������������ �����մϴ�.
	if( get_ability_data()->get_rendering()->animator.get_frame().ix == 0 ){

		if( m_ifire_count >= 3 ){
			set_state( EState::Jump );
		}

	}



}

void CKoopa::jump(){

	int iold_frame_x = get_ability_data()->get_rendering()->animator.get_frame().ix;

	//�ɾҴٰ� �ٴ� ������� ��ȯ�մϴ�.
	//�ٴ� ����϶����� �ڵ����� �ִϸ��̼� �Ǹ� �ȵ˴ϴ�.
	if( iold_frame_x == 0 ){
		get_ability_data()->get_rendering()->animator.update();
		setting_direction();
	}

	//�ٴ� ������� �������� �÷��̾� ��ġ�� ������ �����մϴ�.
	if( iold_frame_x != 1 && get_ability_data()->get_rendering()->animator.get_frame().ix == 1 ){

		CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
		if( pplayer != nullptr ){

			//�÷��̾��� Ÿ�� �ε����� ���ϰ� �� �ε����� ( x��, ������ )���� �̵� ��ġ�� �����մϴ�.
			Tile_Map::TILE_INDEX tile_index = TILE_MAP->calc_tile_index( pplayer->get_ability_data()->get_position()->position );
			if( TILE_MAP->is_available_index( tile_index ) ){
				m_ptjump_target  = TILE_MAP->calc_tile_position( { tile_index.ix, 5 } );
				m_ptjump_target += TILE_MAP->get_tile_property().vtile_size * 0.5f;
			}

			get_ability_data()->get_physics()->benable_gravity   = false;
			get_ability_data()->get_physics()->benable_collision = false;

		}

	}

	//�ٴ� ����̸� Ÿ������ �̵��մϴ�.
	if( get_ability_data()->get_rendering()->animator.get_frame().ix == 1 ){

		Vector2 vdir = m_ptjump_target - get_ability_data()->get_position()->position;
		normalize( &vdir );

		get_ability_data()->get_position()->position += vdir * 1000.0f  * TIME_MANAGER->get_tick();

		//�ٴ� ��ǿ��� �÷��̾ �ƴ϶� �ٴ� ��ġ�� �ٶ󺾴ϴ�.
		if( vdir.fx < 0.0f ){
			( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_origin();
		}else{
			( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_flip();
		}

		//Ÿ�ٿ� ���������� �������ϴ�.
		if( length_square( m_ptjump_target - get_ability_data()->get_position()->position ) < 10.0f ){

			get_ability_data()->get_position()->position = m_ptjump_target;

			set_state( EState::Push );
		}

	}

	

}

void CKoopa::push(){

	//���� �ð� �ִٰ� �������ϴ�.
	if( m_push_stop_timer.signal( 500, false ) ){

		get_ability_data()->get_physics()->benable_gravity   = true;
		get_ability_data()->get_physics()->benable_collision = true;

		//������ �̾��ٰ� �������� �ƴϰ� �Ǿ����� �� ������ ���� �մϴ�.
		//�� �ڵ�� ������ �ѹ��� �۵��ؾ� �մϴ�.
		if( m_bpush_old_jump && !get_ability_data()->get_physics()->bjump && m_bpush_crush_wall ){

			Ability::setting_small_jump( get_ability_data() );

			crush_wall();

			m_bpush_crush_wall = false;

		}

		//�������� �ƴϰ� ���� ���ŵ� �������� �ִϸ��̼��� �����մϴ�.
		if( !get_ability_data()->get_physics()->bjump && !m_bpush_crush_wall ){
			
			get_ability_data()->get_rendering()->animator.update();

			//������ �ִϸ��̼ǿ� ���������� �� �߻� �������� �̵��մϴ�.
			if( get_ability_data()->get_rendering()->animator.get_frame().ix == 8 ){
				set_state( EState::Fire );
			}

		}

	}

	//�߶����� ���� ó���Դϴ�.
	if( get_ability_data()->get_position()->position.fy > 3000.0f ){
		set_state( EState::Die );
	}

	m_bpush_old_jump = get_ability_data()->get_physics()->bjump;

}

void CKoopa::die(){

	if( m_fdie_quake * m_fdie_quake > 0.05f ){
		
		m_fdie_quake			-= 100.0f * 0.3f * TIME_MANAGER->get_tick();
		m_fdie_quake_direction	*= -1.0f;
		Game_Object::CGame_Object_Manager::CMediator::get_scroll()->set_delta( { m_fdie_quake  * m_fdie_quake_direction, 0.0f } );
		//Game_Object::CGame_Object_Manager::CMediator::get_scroll()->set_delta( { 0.0f, m_fdie_quake  * m_fdie_quake_direction } );

		if( m_fdie_quake * m_fdie_quake < 0.05f ){

			m_fdie_quake = 0.0f;
			Game_Object::CGame_Object_Manager::CMediator::get_scroll()->set_delta( { 0.0f, 0.0f } );

			CSoundMgr::Get_Instance()->StopAll();
			CSoundMgr::Get_Instance()->play_sound( TEXT( "Boss_Clear" ), CSoundMgr::CHANNELID::EFFECT );

		}

	}


	if( m_die_timer.signal( 7000 ) ){
		CPlayer* pplayer = Game_Object::CGame_Object_Manager::CMediator::get_player();
		if( pplayer != nullptr ){
			pplayer->clear_koopa();
			destroy();
		}
	}

}

CKoopa_Fire::CKoopa_Fire() : CUnit{}{

	get_ability_data()->alloc_rendering();
	get_ability_data()->alloc_movement();
	get_ability_data()->alloc_physics();
	get_ability_data()->alloc_tile();
	get_ability_data()->alloc_interaction();
	get_ability_data()->alloc_fight();

	m_render.set_data( get_ability_data() );
	get_ability_data()->get_rendering()->pbitmap	= BITMAP_MANAGER->find_bitmap( TEXT( "Koopa_Fire" ) );
	get_ability_data()->get_rendering()->animator.set_bitmap( get_ability_data()->get_rendering()->pbitmap );
	get_ability_data()->get_rendering()->animator.set_sprite( BITMAP_MANAGER->find_sprite( TEXT( "Koopa_Fire_X" ) ) );
	get_ability_data()->get_rendering()->animator.set_frame( { 0, 0 } );
	get_ability_data()->get_rendering()->animator.set_animation_tick_ms( 50 );
	get_ability_data()->get_rendering()->color_key = RGB( 255, 0, 255 );

	m_movement.set_data( get_ability_data() );
	get_ability_data()->get_movement()->fspeed = 300.0f;
	get_ability_data()->get_movement()->fsliding_speed	   =  0.0f;
	get_ability_data()->get_movement()->vdirection		   =  { 1.0f, 0.0f };
	get_ability_data()->get_movement()->fsliding_max_speed =  800.0f;
	get_ability_data()->get_movement()->fsliding_accel     =  600.0f;

	Ability::initialize_physics( get_ability_data() );

	Ability::initialize_interaction( get_ability_data() );
	get_ability_data()->get_interaction()->benable_push = false;

	Ability::initialize_fight( get_ability_data() );

	Ability::update_tile_index( get_ability_data() );


}

Game_Object::CGame_Object::Update_Result CKoopa_Fire::update(){

	m_movement.update();

	if( get_ability_data()->get_position()->vdirection.fx < 0.0f ){
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_origin();
	}else{
		( ( CBidirection_Bitmap* )get_ability_data()->get_rendering()->animator.get_bitmap() )->choice_flip();
	}

	//���� �΋H���� ������ϴ�.
	if( get_ability_data()->get_physics()->bcollision_left ||
		get_ability_data()->get_physics()->bcollision_right || 
		get_ability_data()->get_physics()->bcollision_top || 
		get_ability_data()->get_physics()->bcollision_bottom ){
		//destroy();
	}

	//�΋H���� ���� ó�� �Դϴ�.
	if( get_ability_data()->get_interaction()->bhit_left ||
		get_ability_data()->get_interaction()->bhit_right ||
		get_ability_data()->get_interaction()->bhit_top ||
		get_ability_data()->get_interaction()->bhit_bottom ){

		CPlayer* pplayer = CGame_Object_Manager::CMediator::get_player();
		if( pplayer != nullptr ){
			pplayer->damage();
		}

	}

	Ability::update_tile_index( get_ability_data() );

	get_ability_data()->get_rendering()->animator.update();
	m_render.render();

	return get_update_result();

}
