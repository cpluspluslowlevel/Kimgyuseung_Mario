#ifndef _RPG_ABILITY_H_
#define _RPG_ABILITY_H_

#include "Global.h"
#include "Math.h"
#include "Bitmap.h"
#include "Tile.h"
#include "Game_Object.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN
class CUnit;
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END

API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_BEGIN

enum class EType{
	Position,
	Render,
	Movement,
	Physics,
	Collision
};

struct POSITION_ABILITY_DATA{

	Vector2 position;
	Vector2 vdirection;
	Vector2 vscaling;

	float left() const;
	float right() const;
	float top() const;
	float bottom() const;

	Vector2 left_top() const;
	Vector2 center_top() const;
	Vector2 right_top() const;
	Vector2 left_center() const;
	Vector2 center_center() const;
	Vector2 right_center() const;
	Vector2 left_bottom() const;
	Vector2 center_bottom() const;
	Vector2 right_bottom() const;

};

struct RENDERING_ABILITY_DATA{

	CBitmap_Base* pbitmap;
	CAnimator     animator;
	COLORREF      color_key;

	float fslide_parameter;

};

struct MOVEMENT_ABILITY_DATA{

	float fspeed;

	Vector2 vdirection;

	Vector2 origin;    //이동을 할때의 중점입니다.
					   //특정 중점을 중심으로 이동할때 사용합니다.

	float fsliding_speed;
	float fsliding_max_speed;
	float fsliding_accel;

};

struct PHYSICS_ABILITY_DATA{

	Vector2 old_position;

	bool benable_gravity;
	bool benable_collision;

	bool  bjump;
	float fgravity_time;
	float fjump_y;
	float fjump_speed;
	float faccel_speed;

	bool bcollision_left;
	bool bcollision_right;
	bool bcollision_top;
	bool bcollision_bottom;

	//이 값들을 설정하면 파괴 가능한 벽돌을 파괴할 수 있습니다.
	bool bcrushable_x;
	bool bcrushable_up;


	//이 값들을 설정하면 벽돌에 반응을 줄 수 있습니다.
	bool bhit_x;
	bool bhit_y;
	bool bhit_up;

};

struct TILE_ABILITY_DATA{

	Tile_Map::TILE_INDEX tile_index;
	Tile_Map::TILE_INDEX tile_index_begin;
	Tile_Map::TILE_INDEX tile_index_end;

};

enum class EItem{
	None,
	Mushroom,
	Star,
	Flower,
	Clear,
	Coin,
	Element_Number
};

struct ITEM_ABILITY_DATA{

	bool bacquire;

	EItem item;

	std::weak_ptr< CGame_Object > pcollision_object;

};

struct INTERACTION_ABILITY_DATA{

	bool benable_push;

	//밟거나 밟혔습니다.
	bool bpush;

	//부딫혔습니다.
	bool bhit_left;
	bool bhit_right;
	bool bhit_top;
	bool bhit_bottom;

	std::weak_ptr< CGame_Object > pgame_object;

};

enum class EFight_Attack_Code{
	None,
	Player_Push,
	Player_Sliding,
	Monster_Body
};

struct FIGHT_ABILITY_DATA{

	EFight_Attack_Code code;

	//반응을 활성화 합니다.
	bool benable_attack;
	bool benable_hit;

	//부딫혔습니다.
	bool bhit_left;
	bool bhit_right;
	bool bhit_top;
	bool bhit_bottom;

	Vector2 pt;
	Vector2 vscaling;

	//데미지를 입었습니다.
	bool bdamage;

	std::weak_ptr< CGame_Object > pgame_object;


};

void initialize_physics( CAbility_Data* pdata );
void initialize_interaction( CAbility_Data* pdata );
void initialize_fight( CAbility_Data* pdata );

void reset_interaction( CAbility_Data* pdata );
void reset_fight( CAbility_Data* pdata );

//점프를 설정합니다.
//pdata: 오브젝트의 데이터
void setting_small_jump( CAbility_Data* pdata );
void setting_jump( CAbility_Data* pdata );
void setting_power_jump( CAbility_Data* pdata );
void setting_max_jump( CAbility_Data* pdata );
void setting_normal(  CAbility_Data* pdata );

//점프중 오브젝트가 상승중인지 하락중인지 계산합니다.
bool is_jump_up( const CAbility_Data& data );
bool is_jump_down( const CAbility_Data& data );

//현재 위치에 기반해 주위 타일의 인덱스를 계산합니다.
//pdata: 오브젝트의 데이터. 위치, 타일 어빌리티가 존재해야 합니다.
void update_tile_index( CAbility_Data* pdata );

//어빌리티가 사용하는 데이터들을 관리합니다.
//어빌리티가 사용하는 데이터들은 서로다른 어빌리티가 공유하는 경우가 많습니다.
//그렇기에 데이터 관리자에서 데이터를 관리해주면 서로다른 어빌리티간 종속성을 최소화할 수 있습니다.
class CAbility_Data{
public:
	CAbility_Data();
	CAbility_Data( const CAbility_Data& lvalue );
	CAbility_Data( CAbility_Data&& rvalue ) noexcept;
	~CAbility_Data() noexcept;
	CAbility_Data& operator=( const CAbility_Data& lvalue );
	CAbility_Data& operator=( CAbility_Data&& rvalue ) noexcept;

	//Getter
#pragma region Alloc&Getter
	void								alloc_position();
	POSITION_ABILITY_DATA*				get_position();
	const POSITION_ABILITY_DATA*		get_position() const;

	void								alloc_rendering();
	RENDERING_ABILITY_DATA*				get_rendering();
	const RENDERING_ABILITY_DATA*		get_rendering() const;

	void								alloc_movement();
	MOVEMENT_ABILITY_DATA*				get_movement();
	const MOVEMENT_ABILITY_DATA*		get_movement() const;

	void								alloc_physics();
	PHYSICS_ABILITY_DATA*				get_physics();
	const PHYSICS_ABILITY_DATA*			get_physics() const;

	void								alloc_tile();
	TILE_ABILITY_DATA*					get_tile();
	const TILE_ABILITY_DATA*			get_tile() const;

	void								alloc_item();
	ITEM_ABILITY_DATA*					get_item();
	const ITEM_ABILITY_DATA*			get_item() const;

	void								alloc_interaction();
	INTERACTION_ABILITY_DATA*			get_interaction();
	const INTERACTION_ABILITY_DATA*		get_interaction() const;

	void								alloc_fight();
	FIGHT_ABILITY_DATA*					get_fight();
	const FIGHT_ABILITY_DATA*			get_fight() const;
#pragma endregion

private:
	POSITION_ABILITY_DATA*		m_pposition;
	RENDERING_ABILITY_DATA*		m_prendering;
	MOVEMENT_ABILITY_DATA*		m_pmovement;
	PHYSICS_ABILITY_DATA*		m_pphysics;
	TILE_ABILITY_DATA*			m_ptile;
	ITEM_ABILITY_DATA*			m_pitem;
	INTERACTION_ABILITY_DATA*	m_pinteraction;
	FIGHT_ABILITY_DATA*			m_pfight;
};

//게임 오브젝트가 가진 능력을 나타냅니다.( 전투가능 여부, 대화 가능 여부 등등 )
class CAbility_Base{
public:
	CAbility_Base( EType type );
	CAbility_Base( CAbility_Base& lvalue ) = default;
	CAbility_Base( CAbility_Base&& rvalue ) = default;
	~CAbility_Base() = default;
	CAbility_Base& operator=( const CAbility_Base& ) = default;
	CAbility_Base& operator=( CAbility_Base&& )= default;

	void set_data( CAbility_Data* pdata );

	EType get_type() const;

	CAbility_Data*       get_data();
	const CAbility_Data* get_data() const;

private:

	EType m_type;

	CAbility_Data* m_pdata;

};

//게임 오브젝트에 위치를 가질 수 있는 능력을 부여합니다.
class CPosition_Ability : public CAbility_Base{
public:
	CPosition_Ability();
	CPosition_Ability( const CPosition_Ability& ) = default;
	CPosition_Ability( CPosition_Ability&& ) = default;
	~CPosition_Ability() = default;
	CPosition_Ability& operator=( const CPosition_Ability& ) = default;
	CPosition_Ability& operator=( CPosition_Ability&& ) = default;

	void rotate_direction( float fradian );

	void set( const CPosition_Ability& position );

	void set_position( const Vector2& position );
	void set_position( float fx, float fy );

	void set_scale( const Vector2& scale );
	void set_scale( float fx, float fy );

	void set_direction( const Vector2& scale );
	void set_direction( float fx, float fy );

	const Vector2& get_position() const;
	const Vector2& get_scale() const;
	const Vector2& get_direction() const;

};

//게임 오브젝트에 그림을 그릴 수 있는 능력을 부여합니다.
class CRender_Ability : public CAbility_Base{
public:
	CRender_Ability();
	CRender_Ability( const CRender_Ability& ) = default;
	CRender_Ability( CRender_Ability&& ) = default;
	~CRender_Ability() = default;
	CRender_Ability& operator=( const CRender_Ability& ) = default;
	CRender_Ability& operator=( CRender_Ability&& ) = default;

	virtual void render();

	void render_quad();
	void render_circle();
	void render_direction();
	void render_line();

};

//게임 오브젝트에 애니메이션을 랜더링하는 능력을 부여합니다.
class CBitmap_Render_Ability : public CRender_Ability{
public:
	CBitmap_Render_Ability();
	CBitmap_Render_Ability( const CBitmap_Render_Ability& ) = default;
	CBitmap_Render_Ability( CBitmap_Render_Ability&& ) = default;
	~CBitmap_Render_Ability() = default;
	CBitmap_Render_Ability& operator=( const CBitmap_Render_Ability& ) = default;
	CBitmap_Render_Ability& operator=( CBitmap_Render_Ability&& ) = default;

	//CRender_Ability override
	virtual void render() override;

	void render_slide();

};

//게임 오브젝트에 움직임을 부여합니다.
class CMovement_Ability abstract : public CAbility_Base{
public:
	CMovement_Ability();
	CMovement_Ability( const CMovement_Ability& ) = default;
	CMovement_Ability( CMovement_Ability&& ) = default;
	~CMovement_Ability() = default;
	CMovement_Ability& operator=( const CMovement_Ability& ) = default;
	CMovement_Ability& operator=( CMovement_Ability&& ) = default;

	virtual void update() = 0;

	void set_speed( float fspeed );

	float get_speed() const;

};

//게임 오브젝트에 단방향 이동을 부여합니다.
class CDirection_Movement_Ability : public CMovement_Ability{
public:
	CDirection_Movement_Ability() = default;
	CDirection_Movement_Ability( const CDirection_Movement_Ability& ) = default;
	CDirection_Movement_Ability( CDirection_Movement_Ability&& ) = default;
	~CDirection_Movement_Ability() = default;
	CDirection_Movement_Ability& operator=( const CDirection_Movement_Ability& ) = default;
	CDirection_Movement_Ability& operator=( CDirection_Movement_Ability&& ) = default;
	
	virtual void update() override;

	void move_front();
	void move_back();

};

//게임 오브젝트에 횡스크롤 게임 이동을 부여합니다.
class CBelt_Scroll_Movement_Ability : public CMovement_Ability{
public:
	CBelt_Scroll_Movement_Ability() = default;
	CBelt_Scroll_Movement_Ability( const CBelt_Scroll_Movement_Ability& ) = default;
	CBelt_Scroll_Movement_Ability( CBelt_Scroll_Movement_Ability&& ) = default;
	~CBelt_Scroll_Movement_Ability() = default;
	CBelt_Scroll_Movement_Ability& operator=( const CBelt_Scroll_Movement_Ability& ) = default;
	CBelt_Scroll_Movement_Ability& operator=( CBelt_Scroll_Movement_Ability&& ) = default;
	
	virtual void update() override;

	void move_left();
	void move_right();
	bool sliding();

};

//게임 오브젝트에 좌우 이동을 부여합니다.
//앞이 막히면 방향을 뒤집습니다.
class CX_Direction_Movement : public CBelt_Scroll_Movement_Ability{
public:
	CX_Direction_Movement();
	CX_Direction_Movement( const CX_Direction_Movement& ) = default;
	CX_Direction_Movement( CX_Direction_Movement&& ) = default;
	~CX_Direction_Movement() = default;
	CX_Direction_Movement& operator=( const CX_Direction_Movement& ) = default;
	CX_Direction_Movement& operator=( CX_Direction_Movement&& ) = default;
	
	virtual void update() override;

private:
	
	bool m_bleft_move;

};

//게임 오브젝트에 일반적인 몬스터의 이동을 부여합니다.
//앞이 막히거나 앞에 서있을 발판이 없으면 방향을 뒤집습니다.
class CAI_Movement : public CBelt_Scroll_Movement_Ability{
public:

	enum class EState{
		Normal,
		Y_Check_Normal,			//내 앞에 발판이 없으면 방향을 꺽습니다.
		Jump
	};

public:
	CAI_Movement();
	CAI_Movement( const CAI_Movement& ) = default;
	CAI_Movement( CAI_Movement&& ) = default;
	~CAI_Movement() = default;
	CAI_Movement& operator=( const CAI_Movement& ) = default;
	CAI_Movement& operator=( CAI_Movement&& ) = default;
	
	virtual void update() override;

	void set_left();
	void set_right();
	void set_state( EState state );
	
	bool   is_left_move();
	bool   is_right_move();
	EState get_state() const;

private:

	void move_normal();
	void move_y_check_normal();
	void move_jump();
	
	bool m_bleft_move;

	EState m_state;

};

//물리기능을 지원하는 클래스입니다.
class CPhysics_Ability: public CAbility_Base{
public:
	CPhysics_Ability();
	CPhysics_Ability( const CPhysics_Ability& ) = default;
	CPhysics_Ability( CPhysics_Ability&& ) = default;
	~CPhysics_Ability() = default;
	CPhysics_Ability& operator=( const CPhysics_Ability& ) = default;
	CPhysics_Ability& operator=( CPhysics_Ability&& ) = default;

	void save_data();

};

API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif