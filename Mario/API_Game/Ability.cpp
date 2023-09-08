#include "Ability.h"

#include "Graphics.h"
#include "Time.h"
#include "Wall.h"
#include "Game_Object_Mediator.h"

API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_USING

float POSITION_ABILITY_DATA::left() const{
	return position.fx - vscaling.fx * 0.5f;
}

float POSITION_ABILITY_DATA::right() const{
	return position.fx + vscaling.fx * 0.5f;
}

float POSITION_ABILITY_DATA::top() const{
	return position.fy - vscaling.fy * 0.5f;
}

float POSITION_ABILITY_DATA::bottom() const{
	return position.fy + vscaling.fy * 0.5f;
}

Vector2 POSITION_ABILITY_DATA::left_top() const{
	return { left(), top() };
}

Vector2 POSITION_ABILITY_DATA::center_top() const{
	return { position.fx, top() };
}

Vector2 POSITION_ABILITY_DATA::right_top() const{
	return { right(), top() };
}

Vector2 POSITION_ABILITY_DATA::left_center() const{
	return { left(), position.fy };
}

Vector2 POSITION_ABILITY_DATA::center_center() const{
	return { position.fx, position.fy };
}

Vector2 POSITION_ABILITY_DATA::right_center() const{
	return { right(), position.fy };
}

Vector2 POSITION_ABILITY_DATA::left_bottom() const{
	return { left(), bottom() };
}

Vector2 POSITION_ABILITY_DATA::center_bottom() const{
	return { position.fx, bottom() };
}

Vector2 POSITION_ABILITY_DATA::right_bottom() const{
	return { right(), bottom() };
}


CAbility_Data::CAbility_Data() : m_pposition( nullptr ),
								 m_prendering{ nullptr },
								 m_pmovement{ nullptr },
								 m_pphysics{ nullptr },
								 m_ptile{ nullptr },
								 m_pitem{ nullptr },
								 m_pinteraction{ nullptr },
								 m_pfight{ nullptr }{
}

CAbility_Data::CAbility_Data( const CAbility_Data& lvalue ) : m_pposition{ nullptr },
															  m_prendering{ nullptr },
															  m_pmovement{ nullptr },
															  m_pphysics{ nullptr },
															  m_ptile{ nullptr },
															  m_pitem{ nullptr },
															  m_pinteraction{ nullptr },
															  m_pfight{ nullptr }{

	if( lvalue.m_pposition != nullptr ){
		alloc_position();
		*m_pposition = *lvalue.m_pposition;
	}

	if( lvalue.m_prendering != nullptr ){
		alloc_rendering();
		*m_prendering = *lvalue.m_prendering;
	}

	if( lvalue.m_pmovement != nullptr ){
		alloc_movement();
		*m_pmovement = *lvalue.m_pmovement;
	}

	if( lvalue.m_pphysics != nullptr ){
		alloc_physics();
		*m_pphysics = *lvalue.m_pphysics;
	}

	if( lvalue.m_ptile != nullptr ){
		alloc_tile();
		*m_ptile = *lvalue.m_ptile;
	}

	if( lvalue.m_pitem != nullptr ){
		alloc_item();
		*m_pitem = *lvalue.m_pitem;
	}

	if( lvalue.m_pinteraction != nullptr ){
		alloc_interaction();
		*m_pinteraction = *lvalue.m_pinteraction;
	}

	if( lvalue.m_pfight != nullptr ){
		alloc_fight();
		*m_pfight = *lvalue.m_pfight;
	}

}
CAbility_Data::CAbility_Data( CAbility_Data&& rvalue ) noexcept : m_pposition( rvalue.m_pposition ),
																  m_prendering{ rvalue.m_prendering },
																  m_pmovement{ rvalue.m_pmovement },
																  m_pphysics{ rvalue.m_pphysics },
																  m_ptile{ rvalue.m_ptile },
																  m_pitem{ rvalue.m_pitem },
																  m_pinteraction{ rvalue.m_pinteraction },
																  m_pfight{ rvalue.m_pfight }{
	rvalue.m_pposition		= nullptr;
	rvalue.m_prendering		= nullptr;
	rvalue.m_pmovement		= nullptr;
	rvalue.m_pphysics		= nullptr;
	rvalue.m_ptile			= nullptr;
	rvalue.m_pitem			= nullptr;
	rvalue.m_pinteraction	= nullptr;
	rvalue.m_pfight			= nullptr;
}

CAbility_Data::~CAbility_Data() noexcept{
	Library::safe_delete( m_pposition );
	Library::safe_delete( m_prendering );
	Library::safe_delete( m_pmovement );
	Library::safe_delete( m_pphysics );
	Library::safe_delete( m_ptile );
	Library::safe_delete( m_pitem );
	Library::safe_delete( m_pfight );
}

CAbility_Data& CAbility_Data::operator=( const CAbility_Data& lvalue ){

	if( lvalue.m_pposition != nullptr ){
		if( m_pposition == nullptr ){
			alloc_position();
		}
		*m_pposition = *lvalue.m_pposition;
	}

	if( lvalue.m_prendering != nullptr ){
		if( m_prendering == nullptr ){
			alloc_rendering();
		}
		*m_prendering = *lvalue.m_prendering;
	}

	if( lvalue.m_pmovement != nullptr ){
		if( m_pmovement == nullptr ){
			alloc_movement();
		}
		*m_pmovement = *lvalue.m_pmovement;
	}

	if( lvalue.m_pphysics != nullptr ){
		if( m_pphysics == nullptr ){
			alloc_physics();
		}
		*m_pphysics = *lvalue.m_pphysics;
	}

	if( lvalue.m_ptile != nullptr ){
		if( m_ptile == nullptr ){
			alloc_tile();
		}
		*m_ptile = *lvalue.m_ptile;
	}

	if( lvalue.m_pitem != nullptr ){
		if( m_pitem == nullptr ){
			alloc_tile();
		}
		*m_pitem = *lvalue.m_pitem;
	}

	if( lvalue.m_pfight != nullptr ){
		if( m_pfight == nullptr ){
			alloc_tile();
		}
		*m_pfight = *lvalue.m_pfight;
	}

	return *this;

}

CAbility_Data& CAbility_Data::operator=( CAbility_Data&& rvalue ) noexcept{

	Library::safe_delete( m_pposition );
	m_pposition			= rvalue.m_pposition;
	rvalue.m_pposition	= nullptr;

	Library::safe_delete( m_prendering );
	m_prendering		= rvalue.m_prendering;
	rvalue.m_prendering = nullptr;

	Library::safe_delete( m_pmovement );
	m_pmovement		   = rvalue.m_pmovement;
	rvalue.m_pmovement = nullptr;

	Library::safe_delete( m_pphysics );
	m_pphysics		  = rvalue.m_pphysics;
	rvalue.m_pphysics = nullptr;

	Library::safe_delete( m_ptile );
	m_ptile			= rvalue.m_ptile;
	rvalue.m_ptile	= nullptr;

	Library::safe_delete( m_pitem );
	m_pitem		   = rvalue.m_pitem;
	rvalue.m_pitem = nullptr;

	Library::safe_delete( m_pfight );
	m_pfight		= rvalue.m_pfight;
	rvalue.m_pfight = nullptr;

	return *this;

}

void CAbility_Data::alloc_position(){
	assert( m_pposition == nullptr );
	m_pposition = new POSITION_ABILITY_DATA;
}

POSITION_ABILITY_DATA* CAbility_Data::get_position(){
	return m_pposition;
}

const POSITION_ABILITY_DATA* CAbility_Data::get_position() const{
	return m_pposition;
}

void CAbility_Data::alloc_rendering(){
	assert( m_prendering == nullptr );
	m_prendering = new RENDERING_ABILITY_DATA;
}

RENDERING_ABILITY_DATA* CAbility_Data::get_rendering(){
	return m_prendering;
}

const RENDERING_ABILITY_DATA* CAbility_Data::get_rendering() const{
	return m_prendering;
}

void CAbility_Data::alloc_movement(){
	assert( m_pmovement == nullptr );
	m_pmovement = new MOVEMENT_ABILITY_DATA;
}

MOVEMENT_ABILITY_DATA* CAbility_Data::get_movement(){
	return m_pmovement;
}

const MOVEMENT_ABILITY_DATA* CAbility_Data::get_movement() const{
	return m_pmovement;
}

void CAbility_Data::alloc_physics(){
	assert( m_pphysics == nullptr );
	m_pphysics = new PHYSICS_ABILITY_DATA;
}

PHYSICS_ABILITY_DATA* CAbility_Data::get_physics(){
	return m_pphysics;
}

const PHYSICS_ABILITY_DATA* CAbility_Data::get_physics() const{
	return m_pphysics;
}

void CAbility_Data::alloc_tile(){
	assert( m_ptile == nullptr );
	m_ptile = new TILE_ABILITY_DATA;
}

TILE_ABILITY_DATA* CAbility_Data::get_tile(){
	return m_ptile;
}

const TILE_ABILITY_DATA* CAbility_Data::get_tile() const{
	return m_ptile;
}

void CAbility_Data::alloc_item(){
	assert( m_pitem == nullptr );
	m_pitem = new ITEM_ABILITY_DATA;
}

ITEM_ABILITY_DATA* CAbility_Data::get_item(){
	return m_pitem;
}

const ITEM_ABILITY_DATA* CAbility_Data::get_item() const{
	return m_pitem;
}

void CAbility_Data::alloc_interaction(){
	assert( m_pinteraction == nullptr );
	m_pinteraction = new INTERACTION_ABILITY_DATA;
}

INTERACTION_ABILITY_DATA* CAbility_Data::get_interaction(){
	return m_pinteraction;
}

const INTERACTION_ABILITY_DATA* CAbility_Data::get_interaction() const{
	return m_pinteraction;
}

void CAbility_Data::alloc_fight(){
	assert( m_pfight == nullptr );
	m_pfight = new FIGHT_ABILITY_DATA;
}

FIGHT_ABILITY_DATA* CAbility_Data::get_fight(){
	return m_pfight;
}

const FIGHT_ABILITY_DATA* CAbility_Data::get_fight() const{
	return m_pfight;
}

CAbility_Base::CAbility_Base( EType type ) : m_type{ type },
											 m_pdata( nullptr ){
}

void CAbility_Base::set_data( CAbility_Data* pdata ){
	m_pdata = pdata;
}

EType CAbility_Base::get_type() const {
	return m_type;
}

CAbility_Data* CAbility_Base::get_data(){
	return m_pdata;
}

const CAbility_Data* CAbility_Base::get_data() const{
	return m_pdata;
}

CPosition_Ability::CPosition_Ability() : CAbility_Base{ EType::Position }{
}

void CPosition_Ability::rotate_direction( float fradian ){

	Vector2 direction = get_direction();

	float fradian_y = fradian + 3.141592f * 0.5f;
	Vector2 basic_x{ cosf( fradian ), sinf( fradian ) };
	Vector2 basic_y{ cosf( fradian_y ), sinf( fradian_y ) };

	get_data()->get_position()->vdirection.fx = direction.fx * basic_x.fx + direction.fy * basic_y.fx;
	get_data()->get_position()->vdirection.fy = direction.fx * basic_x.fy + direction.fy * basic_y.fy;

}

void CPosition_Ability::set( const CPosition_Ability& position ){
	set_position( position.get_position() );
	set_scale( position.get_scale() );
}

void CPosition_Ability::set_position( const Vector2& position ){
	set_position( position.fx, position.fy );
}

void CPosition_Ability::set_position( float fx, float fy ){
	get_data()->get_position()->position.fx = fx;
	get_data()->get_position()->position.fy = fy;
}

void CPosition_Ability::set_scale( const Vector2& scale ){
	set_scale( scale.fx, scale.fy );
}

void CPosition_Ability::set_scale( float fx, float fy ){
	get_data()->get_position()->vscaling.fx = fx;
	get_data()->get_position()->vscaling.fy = fy;
}

void CPosition_Ability::set_direction( const Vector2& direction ){
	set_direction( direction.fx, direction.fy );
}

void CPosition_Ability::set_direction( float fx, float fy ){
	get_data()->get_position()->vdirection.fx = fx;
	get_data()->get_position()->vdirection.fy = fy;
}

const Vector2& CPosition_Ability::get_position() const{
	return get_data()->get_position()->position;
}

const Vector2& CPosition_Ability::get_scale() const{
	return get_data()->get_position()->vscaling;;
}

const Vector2& CPosition_Ability::get_direction() const{
	return get_data()->get_position()->vdirection;
}

CRender_Ability::CRender_Ability() : CAbility_Base{ EType::Render }{
}

void CRender_Ability::render(){
	render_quad();
}

void CRender_Ability::render_quad(){

	Graphics::CGraphics* pgraphics = Library::CSingleton< Graphics::CGraphics >::get();

	Vector2			position  = get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt();
	const Vector2&	scale	  = get_data()->get_position()->vscaling;

    HDC hdc = pgraphics->get_dc();

	Rectangle( hdc, ( int )( position.fx - ( scale.fx * 0.5f ) ), ( int )( position.fy - ( scale.fy * 0.5f ) ),
				    ( int )( position.fx + ( scale.fx * 0.5f ) ), ( int )( position.fy + ( scale.fy * 0.5f ) ) );

}

void CRender_Ability::render_circle(){

	Graphics::CGraphics* pgraphics = Library::CSingleton< Graphics::CGraphics >::get();

	Vector2			position  = get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt();
	const Vector2&	scale	  = get_data()->get_position()->vscaling;

    HDC hdc = pgraphics->get_dc();

	Ellipse( hdc, ( int )( position.fx - ( scale.fx * 0.5f ) ), ( int )( position.fy - ( scale.fy * 0.5f ) ),
				  ( int )( position.fx + ( scale.fx * 0.5f ) ), ( int )( position.fy + ( scale.fy * 0.5f ) ) );

}

void CRender_Ability::render_direction(){

	Graphics::CGraphics* pgraphics = Library::CSingleton< Graphics::CGraphics >::get();
	HDC hdc = pgraphics->get_dc();

	Vector2			position  = get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt();
	const Vector2&	direction = get_data()->get_position()->vdirection;

	MoveToEx( hdc, ( int )position.fx, ( int )position.fy, nullptr );
	LineTo( hdc, ( int )( position.fx + direction.fx * 50.0f ), ( int )( position.fy + direction.fy * 50.0f ) );

}

void CRender_Ability::render_line(){

	Graphics::CGraphics* pgraphics = Library::CSingleton< Graphics::CGraphics >::get();
	HDC hdc = pgraphics->get_dc();

	Vector2			position  = get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt();
	const Vector2&	direction = get_data()->get_position()->vdirection;

	MoveToEx( hdc, ( int )position.fx, ( int )position.fy, nullptr );
	LineTo( hdc, ( int )( position.fx + direction.fx ), ( int )( position.fy + direction.fy ) );

}

CBitmap_Render_Ability::CBitmap_Render_Ability() : CRender_Ability{}{
}

void CBitmap_Render_Ability::render(){

	Graphics::CGraphics* pgraphics = Library::CSingleton< Graphics::CGraphics >::get();
	HDC hdc = pgraphics->get_dc();

	Vector2 vscaling = get_data()->get_position()->vscaling;
	Vector2 vposition = get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt() - vscaling * 0.5f;

	get_data()->get_rendering()->animator.render( hdc, 
												  { ( LONG )vposition.fx, ( LONG )vposition.fy, ( LONG )vscaling.fx, ( LONG )vscaling.fy },
												  get_data()->get_rendering()->color_key );

}

void CBitmap_Render_Ability::render_slide(){

	Graphics::CGraphics* pgraphics	      = Library::CSingleton< Graphics::CGraphics >::get();
	HDC					 hdc		      = pgraphics->get_dc();
	HDC					 hbitmap_dc	      = get_data()->get_rendering()->animator.get_bitmap()->get_dc();
	RECT				 bitmap_rect      = get_data()->get_rendering()->animator.calc_bitmap_rect();
	float                fslide_parameter = get_data()->get_rendering()->fslide_parameter;

	Vector2 vscaling	= get_data()->get_position()->vscaling;
	Vector2 vposition	= get_data()->get_position()->position - CGame_Object_Manager::CMediator::get_scroll()->get_pt() - vscaling * 0.5f;

	GdiTransparentBlt( hdc, 
					   ( LONG )vposition.fx, ( LONG )( vposition.fy + vscaling.fy * ( 1.0f - fslide_parameter ) ),
					   ( LONG )vscaling.fx, ( LONG )( vscaling.fy * fslide_parameter ),
					   hbitmap_dc, 
					   bitmap_rect.left, bitmap_rect.top,
					   ( LONG )bitmap_rect.right,
					   ( LONG )( bitmap_rect.bottom * fslide_parameter ),
					   get_data()->get_rendering()->color_key );

}

CMovement_Ability::CMovement_Ability() : CAbility_Base{ EType::Movement }{
}

void CMovement_Ability::set_speed( float fspeed ){
	get_data()->get_movement()->fspeed = fspeed;
}

float CMovement_Ability::get_speed() const{
	return get_data()->get_movement()->fspeed;
}

void CDirection_Movement_Ability::update(){
	move_front();
}

void CDirection_Movement_Ability::move_front(){

	POSITION_ABILITY_DATA* pposition_ability_data	= get_data()->get_position();
	Vector2&               position					= pposition_ability_data->position;
	Vector2&               scale					= pposition_ability_data->vscaling;
	Vector2&               direction				= pposition_ability_data->vdirection;

	MOVEMENT_ABILITY_DATA* pmovement_ability_data = get_data()->get_movement();
	float                  fspeed                 = pmovement_ability_data->fspeed;

	position.fx += direction.fx * fspeed * TIME_MANAGER->get_tick();
	position.fy += direction.fy * fspeed * TIME_MANAGER->get_tick();

}

void CDirection_Movement_Ability::move_back(){

	POSITION_ABILITY_DATA* pposition_ability_data = get_data()->get_position();
	Vector2&                    position		  = pposition_ability_data->position;
	Vector2&                    scale			  = pposition_ability_data->vscaling;
	Vector2&                    direction         = pposition_ability_data->vdirection;

	MOVEMENT_ABILITY_DATA* pmovement_ability_Data = get_data()->get_movement();
	float                  fspeed                 = pmovement_ability_Data->fspeed;

	position.fx -= direction.fx * fspeed * TIME_MANAGER->get_tick();
	position.fy -= direction.fy * fspeed * TIME_MANAGER->get_tick();

}

void CBelt_Scroll_Movement_Ability::update(){
}

void CBelt_Scroll_Movement_Ability::move_left(){

	POSITION_ABILITY_DATA* pposition_ability_data = get_data()->get_position();
	Vector2&               position				  = pposition_ability_data->position;

	MOVEMENT_ABILITY_DATA* pmovement_ability_data = get_data()->get_movement();
	Vector2&               vdirection             = pmovement_ability_data->vdirection;
	float                  fspeed                 = pmovement_ability_data->fspeed;

	PHYSICS_ABILITY_DATA* pphysics_ability_data = get_data()->get_physics();

	if( !pphysics_ability_data->bjump ){
		position -= vdirection * fspeed * TIME_MANAGER->get_tick();
	}else{
		position.fx -= fspeed * 0.5f * TIME_MANAGER->get_tick();
	}

}

void CBelt_Scroll_Movement_Ability::move_right(){

	POSITION_ABILITY_DATA* pposition_ability_data = get_data()->get_position();
	Vector2&               position				  = pposition_ability_data->position;

	MOVEMENT_ABILITY_DATA* pmovement_ability_data = get_data()->get_movement();
	Vector2&               vdirection             = pmovement_ability_data->vdirection;
	float                  fspeed                 = pmovement_ability_data->fspeed;

	PHYSICS_ABILITY_DATA* pphysics_ability_data = get_data()->get_physics();

	if( !pphysics_ability_data->bjump ){
		position += vdirection * fspeed * TIME_MANAGER->get_tick();
	}else{
		position.fx += fspeed * 0.5f * TIME_MANAGER->get_tick();
	}

}

bool CBelt_Scroll_Movement_Ability::sliding(){

	POSITION_ABILITY_DATA* pposition_ability_data = get_data()->get_position();
	Vector2&               position				  = pposition_ability_data->position;

	MOVEMENT_ABILITY_DATA* pmovement_ability_data = get_data()->get_movement();
	Vector2&               vdirection             = pmovement_ability_data->vdirection;
	float                  fspeed                 = pmovement_ability_data->fspeed;

	PHYSICS_ABILITY_DATA* pphysics_ability_data = get_data()->get_physics();

	if( get_data()->get_movement()->vdirection.fy == 0.0f ) return false;

	if( !pphysics_ability_data->bjump ){

		if( vdirection.fy < 0 ){
			position -= vdirection * get_data()->get_movement()->fsliding_speed * TIME_MANAGER->get_tick();
		}else{
			position += vdirection * get_data()->get_movement()->fsliding_speed * TIME_MANAGER->get_tick();
		}

		get_data()->get_movement()->fsliding_speed += get_data()->get_movement()->fsliding_accel * TIME_MANAGER->get_tick();
		get_data()->get_movement()->fsliding_speed  = min( get_data()->get_movement()->fsliding_speed, get_data()->get_movement()->fsliding_max_speed );

		return true;

	}

	return false;

}

CX_Direction_Movement::CX_Direction_Movement() : CBelt_Scroll_Movement_Ability{},
											     m_bleft_move{ false }{
}

void CX_Direction_Movement::update(){

	if( m_bleft_move ){
		move_left();
	}else{
		move_right();
	}

	Vector2	pt = get_data()->get_position()->position;
	if( m_bleft_move ){
		pt.fx -= get_data()->get_position()->vscaling.fx * 0.5f;
	}else{
		pt.fx += get_data()->get_position()->vscaling.fx  * 0.5f;
	}

	Tile_Map::TILE_INDEX index = TILE_MAP->calc_tile_index( pt );
	if( TILE_MAP->is_available_index( index ) ){

		std::weak_ptr< CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( index );
		auto                          ptile      = pweak_tile.lock();

		if( ptile != nullptr && Tile_Map::is_quad_collision( ( ( Unit::CTile* )ptile.get() )->get_tile().collision_type ) ){
			m_bleft_move = !m_bleft_move;
		}

	}

}


CAI_Movement::CAI_Movement() : CBelt_Scroll_Movement_Ability{},
							   m_bleft_move{ false },
							   m_state{ EState::Normal }{
}

void CAI_Movement::update(){

	switch( m_state ){
	case EState::Normal:
		move_normal();
		break;
	case EState::Y_Check_Normal:
		move_y_check_normal();
		break;
	case EState::Jump:
		move_jump();
		break;
	}

}

void CAI_Movement::set_left(){
	m_bleft_move = true;
}

void CAI_Movement::set_right(){
	m_bleft_move = false;
}

void CAI_Movement::set_state( EState state ){
	m_state = state;
}

bool CAI_Movement::is_left_move(){
	return m_bleft_move;
}

bool CAI_Movement::is_right_move(){
	return !m_bleft_move;
}

CAI_Movement::EState CAI_Movement::get_state() const{
	return m_state;
}

void CAI_Movement::move_normal(){

	if( m_bleft_move ){
		move_left();
	}else{
		move_right();
	}

	//앞의 타일을 검사합니다.
	Vector2	pt = get_data()->get_position()->position;
	if( m_bleft_move ){
		pt.fx -= get_data()->get_position()->vscaling.fx * 0.5f;
	}else{
		pt.fx += get_data()->get_position()->vscaling.fx  * 0.5f;
	}

	Tile_Map::TILE_INDEX index = TILE_MAP->calc_tile_index( pt );
	if( TILE_MAP->is_available_index( index ) ){
	
		std::weak_ptr< CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( index );
		auto                          ptile      = pweak_tile.lock();

		//앞에 타일이 있다면 방향을 틀어줍니다.
		if( ptile != nullptr && Tile_Map::is_quad_collision( ( ( Unit::CTile* )ptile.get() )->get_tile().collision_type ) ){
			m_bleft_move = !m_bleft_move;
		}

	}

}

void CAI_Movement::move_y_check_normal(){

	if( m_bleft_move ){
		move_left();
	}else{
		move_right();
	}

	//앞의 타일을 검사합니다.
	//앞의 밑 타일도 검사합니다.
	Vector2	ptfront		 = get_data()->get_position()->position;
	Tile_Map::TILE_INDEX front_down_index = get_data()->get_tile()->tile_index;
	if( m_bleft_move ){
		ptfront.fx		-= get_data()->get_position()->vscaling.fx * 0.5f;
		front_down_index.ix -= 1;
	}else{
		ptfront.fx		+= get_data()->get_position()->vscaling.fx * 0.5f;
		front_down_index.ix += 1;
	}
	front_down_index.iy += 1;

	Tile_Map::TILE_INDEX index_front	  = TILE_MAP->calc_tile_index( ptfront );
	if( TILE_MAP->is_available_index( index_front ) ){

		std::weak_ptr< CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( index_front );
		auto                          ptile      = pweak_tile.lock();
		//앞에 타일이 있다면 방향을 틀어줍니다.
		if( ptile != nullptr && Tile_Map::is_quad_collision( ( ( Unit::CTile* )ptile.get() )->get_tile().collision_type ) ){
			m_bleft_move = !m_bleft_move;
		}

	}

	if( TILE_MAP->is_available_index( front_down_index ) ){

		std::weak_ptr< CGame_Object > pweak_tile = GAME_OBJECT_MANAGER->get_wall_group()->get_tile( front_down_index );
		auto                          ptile      = pweak_tile.lock();

		//타일이 있다면 그 타일이 충돌을 가지지 않으면 방향을 틀어줍니다.
		//없다면 방향을 틀어줍니다.
		if( ptile != nullptr ){
			if( !Tile_Map::is_ground_collision( ( ( Unit::CTile* )ptile.get() )->get_tile().collision_type ) ){
				m_bleft_move = !m_bleft_move;
			}
		}else{
			m_bleft_move = !m_bleft_move;
		}

	}else{
		m_bleft_move = !m_bleft_move;
	}

}

void CAI_Movement::move_jump(){
}

CPhysics_Ability::CPhysics_Ability() : CAbility_Base{ EType::Physics }{
}

void CPhysics_Ability::save_data(){

	get_data()->get_physics()->old_position = get_data()->get_position()->position;

}

void Game_Object::Ability::initialize_physics( CAbility_Data* pdata ){

	pdata->get_physics()->benable_gravity	= true;
	pdata->get_physics()->benable_collision	= true;
	pdata->get_physics()->bjump				= true;
	pdata->get_physics()->fgravity_time		= 0.0f;
	pdata->get_physics()->fjump_y			= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed		= 0.0f;
	pdata->get_physics()->faccel_speed		= 100.0f;
	pdata->get_physics()->bcollision_left	= false;
	pdata->get_physics()->bcollision_right	= false;
	pdata->get_physics()->bcollision_top	= false;
	pdata->get_physics()->bcollision_bottom	= false;
	pdata->get_physics()->bcrushable_x		= false;
	pdata->get_physics()->bcrushable_up		= false;
	pdata->get_physics()->bhit_x			= false;
	pdata->get_physics()->bhit_y			= false;
	pdata->get_physics()->bhit_up			= false;

}

void Game_Object::Ability::initialize_interaction( CAbility_Data* pdata ){

	pdata->get_interaction()->benable_push	= true;
	pdata->get_interaction()->bpush			= false;
	pdata->get_interaction()->bhit_left		= false;
	pdata->get_interaction()->bhit_right	= false;
	pdata->get_interaction()->bhit_top		= false;
	pdata->get_interaction()->bhit_bottom	= false;

}

void Game_Object::Ability::initialize_fight( CAbility_Data* pdata ){

	pdata->get_fight()->code			= EFight_Attack_Code::None;
	pdata->get_fight()->benable_attack	= false;
	pdata->get_fight()->benable_hit		= true;
	pdata->get_fight()->bhit_left		= false;
	pdata->get_fight()->bhit_right		= false;
	pdata->get_fight()->bhit_top		= false;
	pdata->get_fight()->bhit_bottom		= false;
	pdata->get_fight()->bdamage			= false;

}

void Game_Object::Ability::reset_interaction( CAbility_Data* pdata ){

	pdata->get_interaction()->bpush			= false;
	pdata->get_interaction()->bhit_left		= false;
	pdata->get_interaction()->bhit_right	= false;
	pdata->get_interaction()->bhit_top		= false;
	pdata->get_interaction()->bhit_bottom	= false;

}

void Game_Object::Ability::reset_fight( CAbility_Data* pdata ){

	pdata->get_fight()->code			= EFight_Attack_Code::None;
	pdata->get_fight()->bhit_left		= false;
	pdata->get_fight()->bhit_right		= false;
	pdata->get_fight()->bhit_top		= false;
	pdata->get_fight()->bhit_bottom		= false;
	pdata->get_fight()->bdamage			= false;

}

void Game_Object::Ability::setting_small_jump( CAbility_Data* pdata ){

	pdata->get_physics()->bjump			= true;
	pdata->get_physics()->fgravity_time = 0.0f;
	pdata->get_physics()->fjump_y		= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed   = -300.0f;
	pdata->get_physics()->faccel_speed  = 100.0f;

}

void Game_Object::Ability::setting_jump( CAbility_Data* pdata ){

	pdata->get_physics()->bjump			= true;
	pdata->get_physics()->fgravity_time = 0.0f;
	pdata->get_physics()->fjump_y		= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed	= -600.0f;
	pdata->get_physics()->faccel_speed  = 100.0f;

}

void Game_Object::Ability::setting_power_jump( CAbility_Data* pdata ){

	pdata->get_physics()->bjump			= true;
	pdata->get_physics()->fgravity_time = 0.0f;
	pdata->get_physics()->fjump_y		= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed   = -700.0f;
	pdata->get_physics()->faccel_speed  = 100.0f;

}

void Game_Object::Ability::setting_max_jump( CAbility_Data* pdata ){

	pdata->get_physics()->bjump			= true;
	pdata->get_physics()->fgravity_time = 0.0f;
	pdata->get_physics()->fjump_y		= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed   = -800.0f;
	pdata->get_physics()->faccel_speed  = 100.0f;

}

void Game_Object::Ability::setting_normal( CAbility_Data* pdata ){

	pdata->get_physics()->bjump			= false;
	pdata->get_physics()->fgravity_time = 0.0f;
	pdata->get_physics()->fjump_y		= pdata->get_position()->position.fy;
	pdata->get_physics()->fjump_speed   = 300.0f;
	pdata->get_physics()->faccel_speed  = 100.0f;

}

bool Game_Object::Ability::is_jump_up( const CAbility_Data& data ){
	return !is_jump_down( data );
}

bool Game_Object::Ability::is_jump_down( const CAbility_Data& data ){
	//y = at^2 + vt + y0 가속도가 a( 정확히는 가속도 나누기 2 )이고 시작 속도가 v이고 시작위치가 y0일 때 y의 위치값 함수
	//y' = 2at + v       위 함수를 미분하면 y의 위치값 함수는 y의 속도가 된다.
	return 2.0f * data.get_physics()->faccel_speed * 4.9f * data.get_physics()->fgravity_time + data.get_physics()->fjump_speed > 0.0f;
}

void Game_Object::Ability::update_tile_index( CAbility_Data* pdata ){
	
	Vector2 ptbegin = pdata->get_position()->position - pdata->get_position()->vscaling * 0.5f;
	Vector2 ptend   = pdata->get_position()->position + pdata->get_position()->vscaling * 0.5f;

	pdata->get_tile()->tile_index       = TILE_MAP->calc_tile_index(  pdata->get_position()->position );
	pdata->get_tile()->tile_index_begin = TILE_MAP->calc_tile_index( ptbegin );
	pdata->get_tile()->tile_index_end	= TILE_MAP->calc_tile_index( ptend );

	//확실히 하기위해 여유분을 줍니다.
	pdata->get_tile()->tile_index_begin.ix -= pdata->get_tile()->tile_index_begin.ix == 0 ? 0 : 1;
	pdata->get_tile()->tile_index_begin.iy -= pdata->get_tile()->tile_index_begin.iy == 0 ? 0 : 1;;
	pdata->get_tile()->tile_index_end.ix   += 1;
	pdata->get_tile()->tile_index_end.iy   += 1;

	//경계처리를 해줍니다.
	const Tile_Map::TILE_MAP_PROPERTY& tile_map_property = TILE_MAP->get_tile_map_property();
	pdata->get_tile()->tile_index_begin.ix = max( min( pdata->get_tile()->tile_index_begin.ix, tile_map_property.tile_map_size.ix - 1 ), 0 );
	pdata->get_tile()->tile_index_begin.iy = max( min( pdata->get_tile()->tile_index_begin.iy, tile_map_property.tile_map_size.iy - 1 ), 0 );
	pdata->get_tile()->tile_index_end.ix = max( min( pdata->get_tile()->tile_index_end.ix, tile_map_property.tile_map_size.ix - 1 ), 0 );
	pdata->get_tile()->tile_index_end.iy = max( min( pdata->get_tile()->tile_index_end.iy, tile_map_property.tile_map_size.iy - 1 ), 0 );

}