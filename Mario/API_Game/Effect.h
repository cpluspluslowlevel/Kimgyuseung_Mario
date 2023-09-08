#ifndef _API_GAME_EFFECT_H_
#define _API_GAME_EFFECT_H_

#include "Global.h"
#include "Unit.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

class CEffect_Base : public CUnit{
public:
	CEffect_Base();
	CEffect_Base( const CEffect_Base& ) = default;
	CEffect_Base( CEffect_Base&& ) = default;
	virtual ~CEffect_Base() = default;
	CEffect_Base& operator=( const CEffect_Base& ) = default;
	CEffect_Base& operator=( CEffect_Base&& ) = default;

	//CGame_Object override
	virtual Update_Result update() override;

private:

	//Ability
	Ability::CBitmap_Render_Ability m_render;

};

class CCoin_Effect : public CEffect_Base{
public:
	CCoin_Effect();
	CCoin_Effect( const CCoin_Effect& ) = default;
	CCoin_Effect( CCoin_Effect&& ) = default;
	virtual ~CCoin_Effect() = default;
	CCoin_Effect& operator=( const CCoin_Effect& ) = default;
	CCoin_Effect& operator=( CCoin_Effect&& ) = default;

	//CGame_Object override
	virtual Update_Result update() override;

private:

	bool m_binverse_animation;

};

class CWall_Effect: public CEffect_Base{
public:
	CWall_Effect();
	CWall_Effect( const CWall_Effect& ) = default;
	CWall_Effect( CWall_Effect&& ) = default;
	virtual ~CWall_Effect() = default;
	CWall_Effect& operator=( const CWall_Effect& ) = default;
	CWall_Effect& operator=( CWall_Effect&& ) = default;

	//CGame_Object override
	virtual Update_Result update() override;

private:
	Ability::CDirection_Movement_Ability m_movement;

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END


#endif