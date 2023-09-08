#ifndef _API_GAME_GAME_OBJECT_H_
#define _API_GAME_GAME_OBJECT_H_

#include "Global.h"
#include "Tile.h"

API_GAME_GAME_OBJECT_NAMESPACE_BEGIN

API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_BEGIN
class CAbility_Data;
API_GAME_GAME_OBJECT_ABILITY_NAMESPACE_END

//게임에 존재하는 모든 존재들이 가지는 기본 클래스입니다.
class CGame_Object{
public:

	enum class EType{
		Player,
		Enemy,
		Item,
		NPC
	};

	typedef DWORD Update_Result;

	static const Update_Result UPDATE_RESULT_GAME_OBJECT_BEGIN	= 0;
	static const Update_Result UPDATE_RESULT_GAME_OBJECT_END	= 100;


	static const Update_Result UPDATE_RESULT_GAME_OBJECT_NONE    = UPDATE_RESULT_GAME_OBJECT_BEGIN + 0;
	static const Update_Result UPDATE_RESULT_GAME_OBJECT_DESTROY = UPDATE_RESULT_GAME_OBJECT_BEGIN + 1;


public:
	CGame_Object();
	CGame_Object( const CGame_Object& lvalue );
	CGame_Object( CGame_Object&& rvalue ) noexcept;
	virtual ~CGame_Object() noexcept;

	//이 함수를 재정의하여 프레임단위 행동을 제공해야합니다.
	//return: 행동 후 게임오브젝트의 상태( 삭제 등등 )를 리턴해야합니다.
	virtual Update_Result update() = 0;

	//이 함수를 재정의하여 프레임단위 후 행동을 제공해야합니다.
	//행동 후 마무리 작업이 필요할때 사용합니다.
	virtual void update_late();

	//오브젝트와 충돌했을 때 처리를 제공해야합니다.
	virtual void hit( Ability::CAbility_Data* pability );

	void set_this( std::weak_ptr< CGame_Object > pweak_this );

	void destroy();

	EType get_type() const;

	Ability::CAbility_Data* get_ability_data();
	const Ability::CAbility_Data* get_ability_data() const;

	Update_Result get_update_result() const;

	std::weak_ptr< CGame_Object > get_this();

protected:

	void set_update_result( Update_Result update_result );

private:

	EType m_type;

	Ability::CAbility_Data* m_pability_data;

	Update_Result m_update_result;

	//게임 오브젝트 매니저는 shared_ptr와 weak_ptr로 포인터를 관리하기 때문에 자기 자신에 대해 처리하기 위해 weak_ptr를 가져야 합니다.
	std::weak_ptr< CGame_Object >  m_pweak_this;

};

//게임 오브젝트를 그룹지어 처리하기위한 클래스입니다.
//이 클래스를 확장해 그룹별 필요한 처리를 합니다.
//게임오브젝트는 여러개의 그룹에 속할 수 있습니다.
class CGame_Object_Group{
public:
	
	typedef std::list< std::weak_ptr< CGame_Object > >   CGame_Object_Weak_List;

public:
	CGame_Object_Group() = default;
	CGame_Object_Group( const CGame_Object_Group& ) = delete;
	CGame_Object_Group( CGame_Object_Group&& ) = delete;
	~CGame_Object_Group() = default;
	CGame_Object_Group& operator=( const CGame_Object_Group& ) = delete;
	CGame_Object_Group& operator=( CGame_Object_Group&& ) = delete;

	//My virtual
	//그룹단위로 처리할 내용을 작성해야 합니다.
	virtual void update() = 0;


	//원소를 삽입, 삭제합니다.
	//pgame_object: 삽입할 오브젝트
	void insert( std::weak_ptr< CGame_Object > pgame_object );
	void delete_ptr( std::weak_ptr< CGame_Object > pgame_object );
	
	//모든 원소를 지웁니다.
	void clear();

	const CGame_Object_Weak_List& get_list() const;

protected:

	//My virtual
	virtual void insert_process( std::weak_ptr< CGame_Object > pweak_game_object );

	CGame_Object_Weak_List m_pgame_object_weak_list;

};

//중력을 적용받는 게임오브젝트의 그룹입니다.
class CGravity_Unit_Group : public CGame_Object_Group{
public:
	CGravity_Unit_Group() = default;
	CGravity_Unit_Group( const CGravity_Unit_Group& ) = delete;
	CGravity_Unit_Group( CGravity_Unit_Group&& ) = delete;
	~CGravity_Unit_Group() = default;

	virtual void update() override;

};

//벽이랑 충돌하는 게임오브젝트의 그룹입니다.
class CWall_Collision_Unit_Group : public CGame_Object_Group{
public:
	CWall_Collision_Unit_Group() = default;
	CWall_Collision_Unit_Group( const CWall_Collision_Unit_Group& ) = delete;
	CWall_Collision_Unit_Group( CWall_Collision_Unit_Group&& ) = delete;
	~CWall_Collision_Unit_Group() = default;

	virtual void update() override;
};

//벽들의 그룹입니다.
class CWall_Group : public CGame_Object_Group{
public:
	CWall_Group();
	CWall_Group( const CWall_Group& ) = delete;
	CWall_Group( CWall_Group&& ) = delete;
	~CWall_Group() = default;

	virtual void update() override;

	void reset_tile_map();
	void start_tile();

	void render();

	void set_wall_collision_unit_group( const CWall_Collision_Unit_Group* pwall_collision_unit_group );

	std::weak_ptr< CGame_Object > get_tile( const Tile_Map::TILE_INDEX& index );

protected:

	void insert_process( std::weak_ptr< CGame_Object > pweak_game_object );

private:
	const CWall_Collision_Unit_Group* m_pwall_collision_unit_group;

	std::vector< std::weak_ptr< CGame_Object > > m_ptile_vector;

};

//아이템들의 그룹입니다.
class CItem_Group : public CGame_Object_Group{
public:
	CItem_Group() = default;
	CItem_Group( const CItem_Group& ) = delete;
	CItem_Group( CItem_Group&& ) = delete;
	~CItem_Group() = default;
	CItem_Group& operator=( const CItem_Group& ) = delete;
	CItem_Group& operator=( CItem_Group&& ) = delete;

	virtual void update() override;

};

//플레이어에게 밟힐 수 있는 그룹입니다.
class CPush_Group : public CGame_Object_Group{
public:
	CPush_Group() = default;
	CPush_Group( const CPush_Group& ) = delete;
	CPush_Group( CPush_Group&& ) = delete;
	~CPush_Group() = default;
	CPush_Group& operator=( const CPush_Group& ) = delete;
	CPush_Group& operator=( CPush_Group&& ) = delete;

	virtual void update() override;
};

//공격을 하는 오브젝트의 그룹입니다.
class CAttack_Group : public CGame_Object_Group{
public:
	CAttack_Group() = default;
	CAttack_Group( const CAttack_Group& ) = delete;
	CAttack_Group( CAttack_Group&& ) = delete;
	~CAttack_Group() = default;
	CAttack_Group& operator=( const CAttack_Group& ) = delete;
	CAttack_Group& operator=( CAttack_Group&& ) = delete;

	virtual void update() override;

};

//플레이어에게 공격을 받을 수 있는 오브젝트의 그룹입니다.
class CEnemy_Hit_Group : public CGame_Object_Group{
public:
	CEnemy_Hit_Group() = default;
	CEnemy_Hit_Group( const CEnemy_Hit_Group& ) = delete;
	CEnemy_Hit_Group( CEnemy_Hit_Group&& ) = delete;
	~CEnemy_Hit_Group() = default;
	CEnemy_Hit_Group& operator=( const CEnemy_Hit_Group& ) = delete;
	CEnemy_Hit_Group& operator=( CEnemy_Hit_Group&& ) = delete;

	virtual void update() override;

	void set_attack_group( const CAttack_Group* pattack_group );

private:
	const CAttack_Group* m_pattack_group;

};

class CGame_Object_Manager{
public:

	class CMediator;

	enum class EList_Type{
		Environment,
		Wall,
		Unit,
		Player,
		Enemy,
		Bullet,
		Effect,
		Element_Number
	};

	typedef std::list< std::shared_ptr< CGame_Object > > CGame_Object_List;

public:

	bool initialize();
	void release();

	void update();

	//게임 오브젝트를 추가합니다.
	//pgame_object: 추가할 게임 오브젝트
	//list_type: 게임 오브젝트가 들어갈 컨테이너 종류
	std::shared_ptr< CGame_Object > insert( CGame_Object* pgame_object, EList_Type list_type );

	const CGame_Object_List& get_game_object_list( EList_Type type );

	CGravity_Unit_Group*		get_gravity_unit_group();
	CWall_Collision_Unit_Group* get_wall_collision_unit_group();
	CWall_Group*				get_wall_group();
	CItem_Group*                get_item_group();
	CPush_Group*                get_push_group();
	CAttack_Group*				get_attack_group();
	CEnemy_Hit_Group*			get_hit_group();

private:
	SINGLETON_DEFAULT( CGame_Object_Manager );

	CGame_Object_List m_pgame_object_list_array[ ( size_t )EList_Type::Element_Number ];

	CGravity_Unit_Group			m_gravity_unit_group;
	CWall_Collision_Unit_Group	m_wall_collision_unit_group;
	CWall_Group					m_wall_group;
	CItem_Group                 m_item_group;
	CPush_Group                 m_push_group;
	CAttack_Group               m_attack_group;
	CEnemy_Hit_Group			m_hit_group;

};

#define GAME_OBJECT_MANAGER Library::CSingleton< Game_Object::CGame_Object_Manager >::get()

API_GAME_GAME_OBJECT_NAMESPACE_END

#endif