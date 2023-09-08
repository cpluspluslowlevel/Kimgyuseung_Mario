#include "Game_Object.h"

#include "Graphics.h"
#include "Ability.h"
#include "Enemy.h"
#include "Wall.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_NAMESPACE_USING

CGame_Object::CGame_Object() : m_type{},
							   m_pability_data( new Ability::CAbility_Data() ),
							   m_update_result{ UPDATE_RESULT_GAME_OBJECT_NONE },
							   m_pweak_this{}{
}

CGame_Object::CGame_Object( const CGame_Object& lvalue ) : m_type( lvalue.m_type ),
														   m_pability_data( new Ability::CAbility_Data( *lvalue.m_pability_data ) ),
														   m_update_result{ lvalue.m_update_result },
														   m_pweak_this{}{
}

CGame_Object::CGame_Object( CGame_Object&& rvalue ) noexcept : m_type( rvalue.m_type ),
															   m_pability_data( rvalue.m_pability_data ),
															   m_update_result{ rvalue.m_update_result },
															   m_pweak_this{}{
	rvalue.m_pability_data = nullptr;
	rvalue.m_update_result = 0;
}

CGame_Object::~CGame_Object() noexcept{
	delete m_pability_data;
	m_pability_data = nullptr;
}

void CGame_Object::update_late(){
}

void CGame_Object::hit( Ability::CAbility_Data* pability ){
}

void CGame_Object::set_this( std::weak_ptr< CGame_Object > pweak_this ){
	m_pweak_this = pweak_this;
}

void CGame_Object::destroy(){
	set_update_result( CGame_Object::UPDATE_RESULT_GAME_OBJECT_DESTROY );
}

CGame_Object::EType CGame_Object::get_type() const{
	return EType();
}

Game_Object::Ability::CAbility_Data* CGame_Object::get_ability_data(){
	return m_pability_data;
}

const Ability::CAbility_Data* CGame_Object::get_ability_data() const{
	return m_pability_data;
}

CGame_Object::Update_Result CGame_Object::get_update_result() const{
	return m_update_result;
}

std::weak_ptr< CGame_Object > CGame_Object::get_this(){
	return m_pweak_this;
}

void CGame_Object::set_update_result( Update_Result update_result ){
	m_update_result = update_result;
}

void CGame_Object_Group::insert( std::weak_ptr< CGame_Object > pgame_object ){
	m_pgame_object_weak_list.emplace_back( std::move( pgame_object ) );
	insert_process( m_pgame_object_weak_list.back() );
}

void CGame_Object_Group::delete_ptr( std::weak_ptr< CGame_Object > pgame_object ){

	if( pgame_object.expired() ) return;

	auto iter		= m_pgame_object_weak_list.begin();
	auto iter_end	= m_pgame_object_weak_list.end();
	for( ; iter != iter_end; ++iter ){
		
		if( !iter->expired() && iter->lock().get() == pgame_object.lock().get() ){
			m_pgame_object_weak_list.erase( iter );
			return;
		}

	}
}

void CGame_Object_Group::clear(){
	m_pgame_object_weak_list.clear();
}

const CGame_Object_Group::CGame_Object_Weak_List& CGame_Object_Group::get_list() const{
	return m_pgame_object_weak_list;
}

void CGame_Object_Group::insert_process( std::weak_ptr< CGame_Object > pweak_game_object ){
}

void CGravity_Unit_Group::update(){

	for( auto pweak_wall : m_pgame_object_weak_list ){

		if( pweak_wall.expired() ) continue;

		//벽의 정보를 읽어옵니다. 충돌에 필요한 정보가 없다면 넘어갑니다.
		auto							pwall		   = pweak_wall.lock();
		Ability::POSITION_ABILITY_DATA* pwall_position = pwall->get_ability_data()->get_position();
		Ability::PHYSICS_ABILITY_DATA*  pwall_physics  = pwall->get_ability_data()->get_physics();

		if( pwall_position == nullptr || pwall_physics == nullptr ) continue;

		if( pwall_physics->benable_gravity ){
			pwall_position->position.fy = pwall_physics->faccel_speed * 4.9f * pwall_physics->fgravity_time * pwall_physics->fgravity_time + pwall_physics->fjump_speed * pwall_physics->fgravity_time + pwall_physics->fjump_y;
			pwall_physics->fgravity_time += TIME_MANAGER->get_tick();
		}

	}

}

void CWall_Collision_Unit_Group::update(){
}

CWall_Group::CWall_Group() : m_pwall_collision_unit_group{ nullptr },
							 m_ptile_vector{}{
}

void CWall_Group::update(){

	const CGame_Object_Weak_List& unit_list = m_pwall_collision_unit_group->get_list();
	for( auto pweak_ground_unit : unit_list ){

		if( pweak_ground_unit.expired() ) continue;

		//오브젝트의 정보를 읽어옵니다. 충돌에 필요한 정보가 없다면 넘어갑니다.
		auto							punit					= pweak_ground_unit.lock();
		Ability::PHYSICS_ABILITY_DATA*	pground_unit_physics	= punit->get_ability_data()->get_physics();
		pground_unit_physics->bjump				= true;
		pground_unit_physics->bcollision_left   = false;
		pground_unit_physics->bcollision_right  = false;
		pground_unit_physics->bcollision_bottom = false;
		pground_unit_physics->bcollision_top    = false;

		if( !pground_unit_physics->benable_collision ) continue;

		Tile_Map::TILE_INDEX index_start = punit->get_ability_data()->get_tile()->tile_index_begin;
		Tile_Map::TILE_INDEX index_end	 = punit->get_ability_data()->get_tile()->tile_index_end;
		for( size_t iy = index_start.iy; iy <= ( size_t )index_end.iy; ++iy ){
			for( size_t ix = index_start.ix; ix <= ( size_t )index_end.ix; ++ix ){

				auto pwall = m_ptile_vector[ iy * TILE_MAP->get_tile_map_property().tile_map_size.ix + ix ].lock();
				if( pwall != nullptr ){
					( ( Unit::CTile* )pwall.get() )->collision( punit->get_ability_data() );
				}


			}
		}

	}
	
}

void CWall_Group::reset_tile_map(){

	Tile_Map::TILE_MAP_PROPERTY tile_map_property = TILE_MAP->get_tile_map_property();

	m_ptile_vector.resize( tile_map_property.tile_map_size.ix * tile_map_property.tile_map_size.iy );

}

void CWall_Group::start_tile(){

	for( auto pweak : m_ptile_vector ){
		if( !pweak.expired() ){
			( ( Game_Object::Unit::CTile* )pweak.lock().get() )->setting_check();
		}
	}

}

void CWall_Group::render(){

	Vector2						vtile_size		= TILE_MAP->get_tile_property().vtile_size;
	const Tile_Map::TILE_INDEX& vtile_map_size	= TILE_MAP->get_tile_map_property().tile_map_size;

	Vector2 vscroll = Game_Object::CGame_Object_Manager::CMediator::get_scroll()->get_pt();

	size_t istart_x = min( ( size_t )( vscroll.fx / vtile_size.fx ), ( size_t )vtile_map_size.ix - 1 );
	size_t istart_y = min( ( size_t )( vscroll.fy / vtile_size.fy ), ( size_t )vtile_map_size.iy - 1 );
	size_t iend_x   = min( istart_x + ( size_t )( 800.0f / vtile_size.fx ) + 1, ( size_t )vtile_map_size.ix );
	size_t iend_y   = min( istart_y + ( size_t )( 600.0f / vtile_size.fy ) + 1, ( size_t )vtile_map_size.iy );
	for( size_t iy = istart_y; iy < iend_y; ++iy ){
		for( size_t ix = istart_x; ix < iend_x; ++ix ){

			auto ptile = m_ptile_vector[ iy * vtile_map_size.ix +ix ].lock();
			if( ptile != nullptr ){
				( ( Unit::CTile* )ptile.get() )->render();
			}else{
				m_ptile_vector[ iy * vtile_map_size.ix +  ix ] = std::weak_ptr< CGame_Object >();
			}

		}
	}

}

void CWall_Group::set_wall_collision_unit_group( const CWall_Collision_Unit_Group* pwall_collision_unit_group ){
	m_pwall_collision_unit_group = pwall_collision_unit_group;
}

std::weak_ptr< CGame_Object > CWall_Group::get_tile( const Tile_Map::TILE_INDEX& index ){
	assert( TILE_MAP->is_available_index( index ) );
	return m_ptile_vector[ index.iy * TILE_MAP->get_tile_map_property().tile_map_size.ix + index.ix ];
}


void CWall_Group::insert_process( std::weak_ptr< CGame_Object > pweak_game_object ){

	Tile_Map::TILE_MAP_PROPERTY tile_map_property = TILE_MAP->get_tile_map_property();
	auto						pgame_object      = pweak_game_object.lock();

	Tile_Map::TILE_INDEX index = ( ( Unit::CTile* )pgame_object.get() )->get_tile().index;
	assert( pgame_object != nullptr );
	assert( TILE_MAP->is_available_index( index ) );

	m_ptile_vector[ index.iy * tile_map_property.tile_map_size.ix + index.ix ] = std::move( pweak_game_object );

}

void CItem_Group::update(){

	std::weak_ptr< CGame_Object > pweak_player = GAME_OBJECT_MANAGER->get_game_object_list( CGame_Object_Manager::EList_Type::Player ).front();

	if( pweak_player.expired() ) return;

	auto pplayer = pweak_player.lock();
	pplayer->get_ability_data()->get_item()->bacquire = false;

	for( auto pweak_item : m_pgame_object_weak_list ){

		if( pweak_item.expired() ) continue;

		//아이템의 정보를 읽어옵니다. 충돌에 필요한 정보가 없다면 넘어갑니다.
		auto							pitem		   = pweak_item.lock();
		Ability::POSITION_ABILITY_DATA* pitem_position = pitem->get_ability_data()->get_position();
		Ability::PHYSICS_ABILITY_DATA*  pitem_physics  = pitem->get_ability_data()->get_physics();

		if( pitem_position == nullptr || pitem_physics == nullptr ) continue;

		if( CGame_Object_Manager::CMediator::intersect_rect( pplayer->get_ability_data(), pitem->get_ability_data() ) ){
			( ( Unit::CItem_Base* )pitem.get() )->hit_player( pplayer->get_ability_data() );
		}

	}

}

void CPush_Group::update(){

	std::weak_ptr< CGame_Object > pweak_player = GAME_OBJECT_MANAGER->get_game_object_list( CGame_Object_Manager::EList_Type::Player ).front();

	if( pweak_player.expired() ) return;

	auto pplayer = pweak_player.lock();
	Ability::reset_interaction( pplayer->get_ability_data() );

	if( !pplayer->get_ability_data()->get_fight()->benable_hit ) return;

	for( auto pweak_object : m_pgame_object_weak_list ){

		if( pweak_object.expired() ) continue;

		auto pobject = pweak_object.lock();

		Ability::reset_interaction( pobject->get_ability_data() );

		if( !pobject->get_ability_data()->get_fight()->benable_hit ) continue;

		if( CGame_Object_Manager::CMediator::intersect_rect( pplayer->get_ability_data(), pobject->get_ability_data() ) ){

			pplayer->get_ability_data()->get_interaction()->pgame_object	= pobject;
			pobject->get_ability_data()->get_interaction()->pgame_object	= pplayer;

			Vector2 vsize_delta		= pplayer->get_ability_data()->get_position()->vscaling * 0.5f + pobject->get_ability_data()->get_position()->vscaling * 0.5f;
			Vector2 ptdelta			= pplayer->get_ability_data()->get_position()->position - pobject->get_ability_data()->get_position()->position;
			Vector2 vcollision_size = { vsize_delta.fx - fabs( ptdelta.fx ), vsize_delta.fy - fabs( ptdelta.fy ) };
			if( vcollision_size.fx < vcollision_size.fy ){

				if( ptdelta.fx < 0.0f ){    //플레이어가 왼쪽에 있습니다.

					pplayer->get_ability_data()->get_interaction()->bhit_right = true;
					pobject->get_ability_data()->get_interaction()->bhit_left  = true;

				}else{					  //플레이어가 오른쪽에 있습니다.

					pplayer->get_ability_data()->get_interaction()->bhit_left  = true;
					pobject->get_ability_data()->get_interaction()->bhit_right = true;

				}

			}else{

				if( ptdelta.fy < 0.0f ){    //플레이어가 위쪽에 있습니다.

					pplayer->get_ability_data()->get_interaction()->bhit_bottom = true;
					pobject->get_ability_data()->get_interaction()->bhit_top    = true;

					if( pobject->get_ability_data()->get_interaction()->benable_push && pplayer->get_ability_data()->get_physics()->bjump && Ability::is_jump_down( *pplayer->get_ability_data() ) ){
						pplayer->get_ability_data()->get_interaction()->bpush = true;
						pobject->get_ability_data()->get_interaction()->bpush = true;
					}

				}else{					  //플레이어가 아래쪽에 있습니다.

					pplayer->get_ability_data()->get_interaction()->bhit_top    = true;
					pobject->get_ability_data()->get_interaction()->bhit_bottom = true;

				}

			}

		}

	}

}

void CAttack_Group::update(){
}

void CEnemy_Hit_Group::update(){

	const CGame_Object_Weak_List& attack_list = m_pattack_group->get_list();
	for( auto pweak_attack : attack_list ){

		if( pweak_attack.expired() ) continue;
		auto pattack = pweak_attack.lock();
		if( !pattack->get_ability_data()->get_fight()->benable_attack ) continue;
		Ability::reset_fight( pattack->get_ability_data() );

		for( auto pweak_hit : m_pgame_object_weak_list ){

			if(  pweak_hit.expired() ) continue;
			auto phit = pweak_hit.lock();
			if( !phit->get_ability_data()->get_fight()->benable_hit || pattack == phit ) continue;
			Ability::reset_fight( phit->get_ability_data() );

			if( CGame_Object_Manager::CMediator::intersect_rect( phit->get_ability_data(), pattack->get_ability_data() ) ){

				pattack->hit( phit->get_ability_data() );
				phit->hit( pattack->get_ability_data() );
				
				phit->get_ability_data()->get_fight()->bdamage = true;

				pattack->get_ability_data()->get_fight()->pgame_object	= phit;
				pattack->get_ability_data()->get_fight()->pt			= pattack->get_ability_data()->get_position()->position;
				pattack->get_ability_data()->get_fight()->vscaling		= pattack->get_ability_data()->get_position()->vscaling;
				phit->get_ability_data()->get_fight()->pgame_object		= pattack;
				phit->get_ability_data()->get_fight()->pt				= phit->get_ability_data()->get_position()->position;
				phit->get_ability_data()->get_fight()->vscaling			= phit->get_ability_data()->get_position()->vscaling;

				Vector2 vsize_delta		= pattack->get_ability_data()->get_position()->vscaling * 0.5f + phit->get_ability_data()->get_position()->vscaling * 0.5f;
				Vector2 ptdelta			= pattack->get_ability_data()->get_position()->position - phit->get_ability_data()->get_position()->position;
				Vector2 vcollision_size = { vsize_delta.fx - fabs( ptdelta.fx ), vsize_delta.fy - fabs( ptdelta.fy ) };
				if( vcollision_size.fx < vcollision_size.fy ){

					if( ptdelta.fx < 0.0f ){    //왼쪽에 있습니다.

						pattack->get_ability_data()->get_fight()->bhit_right = true;
						phit->get_ability_data()->get_fight()->bhit_left     = true;

					}else{					  //오른쪽에 있습니다.

						pattack->get_ability_data()->get_fight()->bhit_left  = true;
						phit->get_ability_data()->get_fight()->bhit_right    = true;

					}

				}else{

					if( ptdelta.fy < 0.0f ){    //위쪽에 있습니다.

						pattack->get_ability_data()->get_fight()->bhit_bottom = true;
						phit->get_ability_data()->get_fight()->bhit_top       = true;

					}else{					  //아래쪽에 있습니다.

						pattack->get_ability_data()->get_fight()->bhit_top = true;
						phit->get_ability_data()->get_fight()->bhit_bottom = true;

					}

				}

			}


		}

	}

}

void CEnemy_Hit_Group::set_attack_group( const CAttack_Group* pattack_group ){
	m_pattack_group = pattack_group;
}

bool CGame_Object_Manager::initialize(){

	m_wall_group.set_wall_collision_unit_group( &m_wall_collision_unit_group );
	m_hit_group.set_attack_group( &m_attack_group );

	return true;

}

void CGame_Object_Manager::release(){

	for( auto& pgame_object_list : m_pgame_object_list_array ){
		pgame_object_list.clear();
	}

	m_gravity_unit_group.clear();
	m_wall_collision_unit_group.clear();
	m_wall_group.clear();
	m_item_group.clear();

}

void CGame_Object_Manager::update(){

	m_wall_group.render();

	for( auto& pgame_object_list : m_pgame_object_list_array ){

		auto iter = pgame_object_list.begin();
		auto end_iter = pgame_object_list.end();
		while( iter != end_iter ){

			( *iter )->update();

			if( ( *iter )->get_update_result() == CGame_Object::UPDATE_RESULT_GAME_OBJECT_DESTROY ){
				iter = pgame_object_list.erase( iter );
			}else{
				++iter;
			}

		}

	}

	for( auto& pgame_object_list : m_pgame_object_list_array ){

		auto iter = pgame_object_list.begin();
		auto end_iter = pgame_object_list.end();
		while( iter != end_iter ){

			( *iter )->update_late();

			++iter;

		}

	}

	m_gravity_unit_group.update();
	m_wall_collision_unit_group.update();
	m_wall_group.update();
	m_item_group.update();
	m_push_group.update();
	m_attack_group.update();
	m_hit_group.update();

}

std::shared_ptr< Game_Object::CGame_Object > CGame_Object_Manager::insert( CGame_Object* pgame_object, EList_Type list_type ){
	m_pgame_object_list_array[ ( size_t )list_type ].emplace_back( pgame_object );
	pgame_object->set_this( m_pgame_object_list_array[ ( size_t )list_type ].back() );
	return m_pgame_object_list_array[ ( size_t )list_type ].back();
}

const CGame_Object_Manager::CGame_Object_List& CGame_Object_Manager::get_game_object_list( EList_Type type ){
	return m_pgame_object_list_array[ ( size_t )type ];
}

CGravity_Unit_Group* CGame_Object_Manager::get_gravity_unit_group(){
	return &m_gravity_unit_group;
}

CWall_Collision_Unit_Group* CGame_Object_Manager::get_wall_collision_unit_group(){
	return &m_wall_collision_unit_group;
}

CWall_Group* CGame_Object_Manager::get_wall_group(){
	return &m_wall_group;
}

CItem_Group* CGame_Object_Manager::get_item_group(){
	return &m_item_group;
}

CPush_Group* CGame_Object_Manager::get_push_group(){
	return &m_push_group;
}

CAttack_Group* CGame_Object_Manager::get_attack_group(){
	return &m_attack_group;
}

CEnemy_Hit_Group* CGame_Object_Manager::get_hit_group(){
	return &m_hit_group;
}