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

	Vector2 origin;    //�̵��� �Ҷ��� �����Դϴ�.
					   //Ư�� ������ �߽����� �̵��Ҷ� ����մϴ�.

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

	//�� ������ �����ϸ� �ı� ������ ������ �ı��� �� �ֽ��ϴ�.
	bool bcrushable_x;
	bool bcrushable_up;


	//�� ������ �����ϸ� ������ ������ �� �� �ֽ��ϴ�.
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

	//��ų� �������ϴ�.
	bool bpush;

	//�΋H�����ϴ�.
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

	//������ Ȱ��ȭ �մϴ�.
	bool benable_attack;
	bool benable_hit;

	//�΋H�����ϴ�.
	bool bhit_left;
	bool bhit_right;
	bool bhit_top;
	bool bhit_bottom;

	Vector2 pt;
	Vector2 vscaling;

	//�������� �Ծ����ϴ�.
	bool bdamage;

	std::weak_ptr< CGame_Object > pgame_object;


};

void initialize_physics( CAbility_Data* pdata );
void initialize_interaction( CAbility_Data* pdata );
void initialize_fight( CAbility_Data* pdata );

void reset_interaction( CAbility_Data* pdata );
void reset_fight( CAbility_Data* pdata );

//������ �����մϴ�.
//pdata: ������Ʈ�� ������
void setting_small_jump( CAbility_Data* pdata );
void setting_jump( CAbility_Data* pdata );
void setting_power_jump( CAbility_Data* pdata );
void setting_max_jump( CAbility_Data* pdata );
void setting_normal(  CAbility_Data* pdata );

//������ ������Ʈ�� ��������� �϶������� ����մϴ�.
bool is_jump_up( const CAbility_Data& data );
bool is_jump_down( const CAbility_Data& data );

//���� ��ġ�� ����� ���� Ÿ���� �ε����� ����մϴ�.
//pdata: ������Ʈ�� ������. ��ġ, Ÿ�� �����Ƽ�� �����ؾ� �մϴ�.
void update_tile_index( CAbility_Data* pdata );

//�����Ƽ�� ����ϴ� �����͵��� �����մϴ�.
//�����Ƽ�� ����ϴ� �����͵��� ���δٸ� �����Ƽ�� �����ϴ� ��찡 �����ϴ�.
//�׷��⿡ ������ �����ڿ��� �����͸� �������ָ� ���δٸ� �����Ƽ�� ���Ӽ��� �ּ�ȭ�� �� �ֽ��ϴ�.
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

//���� ������Ʈ�� ���� �ɷ��� ��Ÿ���ϴ�.( �������� ����, ��ȭ ���� ���� ��� )
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

//���� ������Ʈ�� ��ġ�� ���� �� �ִ� �ɷ��� �ο��մϴ�.
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

//���� ������Ʈ�� �׸��� �׸� �� �ִ� �ɷ��� �ο��մϴ�.
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

//���� ������Ʈ�� �ִϸ��̼��� �������ϴ� �ɷ��� �ο��մϴ�.
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

//���� ������Ʈ�� �������� �ο��մϴ�.
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

//���� ������Ʈ�� �ܹ��� �̵��� �ο��մϴ�.
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

//���� ������Ʈ�� Ⱦ��ũ�� ���� �̵��� �ο��մϴ�.
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

//���� ������Ʈ�� �¿� �̵��� �ο��մϴ�.
//���� ������ ������ �������ϴ�.
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

//���� ������Ʈ�� �Ϲ����� ������ �̵��� �ο��մϴ�.
//���� �����ų� �տ� ������ ������ ������ ������ �������ϴ�.
class CAI_Movement : public CBelt_Scroll_Movement_Ability{
public:

	enum class EState{
		Normal,
		Y_Check_Normal,			//�� �տ� ������ ������ ������ �����ϴ�.
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

//��������� �����ϴ� Ŭ�����Դϴ�.
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