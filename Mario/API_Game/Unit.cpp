#include "Unit.h"

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_USING;

CUnit::CUnit() : m_position{}{
	
	get_ability_data()->alloc_position();

	m_position.set_data( get_ability_data() );

}

Game_Object::Ability::CPosition_Ability* CUnit::get_position(){
	return &m_position;
}
