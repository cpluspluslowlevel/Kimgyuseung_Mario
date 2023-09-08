#ifndef _API_GAME_PLAYER_H_
#define _API_GAME_PLAYER_H_

#include "Bitmap.h"
#include "Unit.h"
#include "Time.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

class CPlayer : public CUnit{
public:

	struct SPRITE_SET{
		
		CBitmap_Base* pbitmap;
		const SPRITE* pstand_sprite;
		const SPRITE* pwalk_sprite;
		const SPRITE* prun_max_sprite;
		const SPRITE* pjump_sprite;
		const SPRITE* ppower_jump_sprite;
		const SPRITE* psliding_sprite;
		const SPRITE* pdie_sprite;

		Vector2 vsize;

	};

	enum class EMode{
		None,
		Small,
		Big,
		Element_Number
	};

	enum class ESprite{
		None,
		Stand,
		Walk,
		Run,
		Run_Max,
		Jump,
		Sliding,
		Die,
		Element_Number
	};

	enum class EControl_Mode{
		Normal,
		Item,
		Pipe,
		Clear,
		Die
	};

	static const Update_Result UPDATE_RESULT_PLAYER_BEGIN	= UPDATE_RESULT_UNIT_END + 1;
	static const Update_Result UPDATE_RESULT_PLAYER_END		= UPDATE_RESULT_PLAYER_BEGIN + 100;

	static const Update_Result UPDATE_RESULT_PLAYER_CLEAR		= UPDATE_RESULT_PLAYER_BEGIN + 0;
	static const Update_Result UPDATE_RESULT_PLAYER_GAME_OVER	= UPDATE_RESULT_PLAYER_BEGIN + 1;
	static const Update_Result UPDATE_RESULT_PLAYER_PIPE		= UPDATE_RESULT_PLAYER_BEGIN + 2;
	static const Update_Result UPDATE_RESULT_PLAYER_DIE			= UPDATE_RESULT_PLAYER_BEGIN + 3;


public:
	CPlayer();
	CPlayer( const CPlayer& ) = delete;
	CPlayer( CPlayer&& ) = delete;
	virtual ~CPlayer() = default;

	//Override
	virtual Update_Result update() override;

	virtual void update_late() override;

	void clear_koopa();
	void damage();

	void set_mode( EMode mode );
	void set_sprite( ESprite state );
	void set_controll_mode( EControl_Mode mode );
	void set_pipe_stage( DWORD dwstage );
	void set_pipe_position( DWORD dwposition );

	int get_accel() const;

	DWORD get_pipe_stage() const;
	DWORD get_pipe_position() const;

	//Globla Data
	static int s_icoin;
	static int s_ilife;
	static int s_iscore;
	static int s_iclear_item[ 3 ];

private:

	void initialize_resource();

	//State
	void change_mode();
	void change_sprite();
	void change_control_mode();

	//Input
	void input_move( DWORD dwleft_key, DWORD dwright_key, DWORD dwaccel_key, DWORD dwsliding_key );
	void input_sliding( DWORD dwsliding_key );
	void input_jump( DWORD dwkey );

	//Control
	void control_normal();
	void control_item();
	void control_clear();
	void control_pipe();
	void control_die();

	void change_state_move();
	void jump();
	void check_item();


	int calc_accel_level() const;

	//Ability
	Ability::CBitmap_Render_Ability			m_render;
	Ability::CBelt_Scroll_Movement_Ability	m_movement;
	Ability::CPhysics_Ability				m_physics;

	//Sprite
	EMode  m_mode;
	EMode  m_old_mode;
	ESprite m_sprite;
	ESprite m_old_sprite;

	SPRITE_SET m_sprite_set[ ( size_t )EMode::Element_Number ];

	bool   m_bleft;
	bool   m_bold_left;

	//Control
	EControl_Mode m_control_mode;
	EControl_Mode m_old_control_mode;
	Time::CTimer  m_clear_timer;

	//Move
	int			 m_iaccel;
	Time::CTimer m_accel_timer;

	//Jump
	bool         m_bjump_pressed;
	Time::CTimer m_pressed_jump_timer;
	float        m_fmax_jump_speed;

	//Pipe
	DWORD m_dwpipe_stage;
	DWORD m_dwpipe_position;

	//Die
	Time::CTimer m_die_move_timer;
	Time::CTimer m_die_scene_move_timer;

	Time::CTimer m_no_damage_timer;
	Time::CTimer m_no_damage_render_timer;
	bool         m_bno_damage;
	bool		 m_bno_damage_render_on;

	

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END


#endif