#ifndef _API_GAME_FIELD_H_
#define _API_GAME_FIELD_H_

#include "Global.h"
#include "Bitmap.h"
#include "Tile.h"
#include "Game_Object.h"

//필드를 나타내는 클래스들의 베이스 입니다.
//필드들이 갖추어야하는 기능들을 정의합니다.
class CField_Base{
public:

    //이 함수를 재정의하여 필요한 리소스들을 준비해야 합니다.
    virtual void initialize() = 0;

    //이 함수를 재정의하여 사용한 리소스들을 정리해야 합니다.
    virtual void release() = 0;

    //이 함수를 재정의하여 프레임단위 행동을 제공해야합니다.
    //return: 필드가 변경될 경우 새로운 필드의 포인터를 리턴합니다.
    //        변경하지 않을경우 this를 리턴합니다.
    //        게임을 종료할 경우 nullptr을 리턴합니다.
    virtual CField_Base* update() = 0;

    //이 함수를 재정의하여 프레임단위 렌더링을 제공해야합니다.
    virtual void render() = 0;

private:
};

//전투 스테이지 입니다.
//적을 생성할 팩토리를 입력받아 스테이지를 구성합니다.
class CFight_Field : public CField_Base{
public:
    CFight_Field();
    CFight_Field( const CFight_Field& ) = delete;
    CFight_Field( CFight_Field&& ) = delete;
    virtual ~CFight_Field() = default;
    CFight_Field& operator=( const CFight_Field& ) = delete;
    CFight_Field& operator=( CFight_Field&& ) = delete;

    virtual void initialize() override;

    virtual void release() override;

    virtual CField_Base* update() override;

    virtual void render() override;

	void set_stage( DWORD dwstage );
	void set_position( DWORD dwposition );
	void set_pipe( bool bpipe );

	DWORD get_stage() const;
	DWORD get_position() const;
	bool  get_pipe() const;

private:

    const char* get_tile_map_path();
    DWORD       get_next_stage() const;
    XY          get_background_tile_index();

    void play_bgm();

    void render_ui();

    bool initialize_resource();
	bool initialize_bitmap();
	bool initialize_sprite();
    bool initialize_sound();


    CAnimator m_background_animation;
    XY        m_background_tile_index;

    CBitmap_Base* m_pui_bitmap;
    CAnimator     m_ui_number_animation;
    CAnimator     m_ui_accel_animation;
    CAnimator     m_ui_accel_full_animation;
    CAnimator     m_ui_clear_item_animation;


	DWORD m_dwstage;
	DWORD m_dwposition;
	bool  m_bpipe;

    static bool s_binitialize_resource;    //리소스는 한번만 불러와야 하기에 정적변수로 표시합니다.

};

//게임 클리어 스테이지 입니다.
class CEnding_Field : public CField_Base{
public:
    CEnding_Field();
    CEnding_Field( const CEnding_Field& ) = delete;
    CEnding_Field( CEnding_Field&& ) = delete;
    virtual ~CEnding_Field() = default;
    CEnding_Field& operator=( const CEnding_Field& ) = delete;
    CEnding_Field& operator=( CEnding_Field&& ) = delete;

    virtual void initialize() override;

    virtual void release() override;

    virtual CField_Base* update() override;

    virtual void render() override;

public:

    CBitmap_Base* m_pbitmap;

};

#endif