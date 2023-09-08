#ifndef _API_GAME_WALL_H_
#define _API_GAME_WALL_H_

#include "Unit.h"
#include "Tile.h"


API_GAME_GAME_OBJECT_NAMESPACE_BEGIN
API_GAME_GAME_OBJECT_UNIT_NAMESPACE_BEGIN

class CTile : public CUnit{
public:
	CTile();
	CTile( const CTile& ) = delete;
	CTile( CTile&& ) = delete;
	~CTile() = default;

	//CGame_Object override
	virtual Update_Result update() override;


	//My virtual
	//��� �ʱ�ȭ�� ������ ���� ���������� ����� �� ȣ��˴ϴ�.
	//��� �ʱ�ȭ �� �ʿ��� �۾��� �����մϴ�.
	virtual void start();

	//������Ʈ���� �浹�� �����ؾ� �մϴ�.
	//pdata: �浹�ϴ��� �˻��� ������Ʈ
	virtual void collision( Ability::CAbility_Data* pdata );

	//������ ������ �� Ÿ���� �μ��� �Ҷ� ó���� �մϴ�.
	//return: ������ ������� ��ü���� ���߱⸦ �䱸�Ҷ� true. �ƴҽ� false�Դϴ�.
	virtual bool push_crush();

	//�������� �����ؾ� �մϴ�.
	virtual void render();

	//��ü�� ������ ������ true �ƴϸ� false�Դϴ�.
	virtual bool is_left_wall();
	virtual bool is_right_wall();
	virtual bool is_top_wall();
	virtual bool is_down_wall();

	//My virtual
	//�� Ÿ�� ���� �ö�ź ������Ʈ���� �̵� ������ �˷�����մϴ�.
	//���ʿ��� ���������� ���� ���� ���͸� �����ؾ� �մϴ�.
	virtual Vector2 get_direction() const;

	//���� Ÿ�Ͽ� ���� ���� �ִٰ� ����, ���� �մϴ�.
	void setting_check();
	void reset_check();

	//Ư�� �������� �˻����� �����մϴ�.
	bool set_left_check( bool bcheck );
	bool set_right_check( bool bcheck );
	bool set_top_check( bool bcheck );
	bool set_bottom_check( bool bcheck );

	void set_tile( const Tile_Map::TILE& tile );

	const Tile_Map::TILE& get_tile() const;

protected:

	//�ֺ��� Ÿ���� ������ �� �κ��� �˻����� �ʽ��ϴ�.
	bool m_bleft_check;
	bool m_bright_check;
	bool m_btop_check;
	bool m_bbottom_check;

private:
	
	Ability::CBitmap_Render_Ability m_tile_render;
	Ability::CPhysics_Ability		m_physics;

	Tile_Map::TILE m_tile;

};

class CWall : public CTile{
public:
	CWall() = default;
	CWall( const CWall& ) = delete;
	CWall( CWall&& ) noexcept = delete;
	~CWall() noexcept = default;
	CWall& operator=( const CWall& ) = delete;
	CWall& operator=( CWall&& ) noexcept = delete;

	virtual void collision( Ability::CAbility_Data* pdata );

	virtual bool push_crush() override;

	//Ư�� �������� ������Ʈ�� ������ �����մϴ�.
	virtual bool is_left_wall() override;
	virtual bool is_right_wall() override;
	virtual bool is_top_wall() override;
	virtual bool is_down_wall() override;

};

class CCrushable_Wall : public CWall{
public:
	CCrushable_Wall();
	CCrushable_Wall( const CCrushable_Wall& ) = delete;
	CCrushable_Wall( CCrushable_Wall&& ) = delete;
	~CCrushable_Wall() = default;


	//CTile override
	virtual void collision( Ability::CAbility_Data* pdata ) override;

	virtual bool push_crush() override;

private:

	void crush_effect();

};

class CItem_Wall : public CWall{
public:
	CItem_Wall();
	CItem_Wall( const CItem_Wall& ) = delete;
	CItem_Wall( CItem_Wall&& ) = delete;
	~CItem_Wall() = default;


	//CWall override
	virtual void collision( Ability::CAbility_Data* pdata ) override;

private:

	void hit();

	bool m_benable;

};

class CQuad_Up_Wall : public CWall{
public:
	CQuad_Up_Wall() = default;
	CQuad_Up_Wall( const CQuad_Up_Wall& ) = delete;
	CQuad_Up_Wall( CQuad_Up_Wall&& ) = delete;
	~CQuad_Up_Wall() = default;

	//CTile override
	virtual void collision( Ability::CAbility_Data* pdata ) override;

};

class CDiagonal : public CWall{
public:
	CDiagonal() = default;
	CDiagonal( const CDiagonal& ) = delete;
	CDiagonal( CDiagonal&& ) = delete;
	~CDiagonal() = default;
	CDiagonal& operator=( const CDiagonal& ) = delete;
	CDiagonal& operator=( CDiagonal&& ) = delete;

	//CTile override
	virtual void collision( Ability::CAbility_Data* pdata ) override;

	//My virtual
	virtual LINE get_diagonal() const = 0;

	virtual Vector2 get_direction() const override;

};

class CDiagonal_Right : public CDiagonal{
public:
	CDiagonal_Right() = default;
	CDiagonal_Right( const CDiagonal_Right& ) = delete;
	CDiagonal_Right( CDiagonal_Right&& ) = delete;
	~CDiagonal_Right() = default;
	CDiagonal_Right& operator=( const CDiagonal_Right& ) = delete;
	CDiagonal_Right& operator=( CDiagonal_Right&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};

class CDiagonal_Right_Bottom : public CDiagonal{
public:
	CDiagonal_Right_Bottom() = default;
	CDiagonal_Right_Bottom( const CDiagonal_Right_Bottom& ) = delete;
	CDiagonal_Right_Bottom( CDiagonal_Right_Bottom&& ) = delete;
	~CDiagonal_Right_Bottom() = default;
	CDiagonal_Right_Bottom& operator=( const CDiagonal_Right_Bottom& ) = delete;
	CDiagonal_Right_Bottom& operator=( CDiagonal_Right_Bottom&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};

class CDiagonal_Right_Middle : public CDiagonal{
public:
	CDiagonal_Right_Middle() = default;
	CDiagonal_Right_Middle( const CDiagonal_Right_Middle& ) = delete;
	CDiagonal_Right_Middle( CDiagonal_Right_Middle&& ) = delete;
	~CDiagonal_Right_Middle() = default;
	CDiagonal_Right_Middle& operator=( const CDiagonal_Right_Middle& ) = delete;
	CDiagonal_Right_Middle& operator=( CDiagonal_Right_Middle&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};

class CDiagonal_Left : public CDiagonal{
public:
	CDiagonal_Left() = default;
	CDiagonal_Left( const CDiagonal_Left& ) = delete;
	CDiagonal_Left( CDiagonal_Left&& ) = delete;
	~CDiagonal_Left() = default;
	CDiagonal_Left& operator=( const CDiagonal_Left& ) = delete;
	CDiagonal_Left& operator=( CDiagonal_Left&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};


class CDiagonal_Left_Bottom : public CDiagonal{
public:
	CDiagonal_Left_Bottom() = default;
	CDiagonal_Left_Bottom( const CDiagonal_Left_Bottom& ) = delete;
	CDiagonal_Left_Bottom( CDiagonal_Left_Bottom&& ) = delete;
	~CDiagonal_Left_Bottom() = default;
	CDiagonal_Left_Bottom& operator=( const CDiagonal_Left_Bottom& ) = delete;
	CDiagonal_Left_Bottom& operator=( CDiagonal_Left_Bottom&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};

class CDiagonal_Left_Middle : public CDiagonal{
public:
	CDiagonal_Left_Middle() = default;
	CDiagonal_Left_Middle( const CDiagonal_Left_Middle& ) = delete;
	CDiagonal_Left_Middle( CDiagonal_Left_Middle&& ) = delete;
	~CDiagonal_Left_Middle() = default;
	CDiagonal_Left_Middle& operator=( const CDiagonal_Left_Middle& ) = delete;
	CDiagonal_Left_Middle& operator=( CDiagonal_Left_Middle&& ) = delete;

	//CDiagonal override
	virtual LINE get_diagonal() const override;

};

API_GAME_GAME_OBJECT_UNIT_NAMESPACE_END
API_GAME_GAME_OBJECT_NAMESPACE_END

#endif