#ifndef _API_GAME_FIELD_H_
#define _API_GAME_FIELD_H_

#include "Global.h"
#include "Bitmap.h"
#include "Tile.h"
#include "Game_Object.h"

//�ʵ带 ��Ÿ���� Ŭ�������� ���̽� �Դϴ�.
//�ʵ���� ���߾���ϴ� ��ɵ��� �����մϴ�.
class CField_Base{
public:

    //�� �Լ��� �������Ͽ� �ʿ��� ���ҽ����� �غ��ؾ� �մϴ�.
    virtual void initialize() = 0;

    //�� �Լ��� �������Ͽ� ����� ���ҽ����� �����ؾ� �մϴ�.
    virtual void release() = 0;

    //�� �Լ��� �������Ͽ� �����Ӵ��� �ൿ�� �����ؾ��մϴ�.
    //return: �ʵ尡 ����� ��� ���ο� �ʵ��� �����͸� �����մϴ�.
    //        �������� ������� this�� �����մϴ�.
    //        ������ ������ ��� nullptr�� �����մϴ�.
    virtual CField_Base* update() = 0;

    //�� �Լ��� �������Ͽ� �����Ӵ��� �������� �����ؾ��մϴ�.
    virtual void render() = 0;

private:
};

//���� �������� �Դϴ�.
//���� ������ ���丮�� �Է¹޾� ���������� �����մϴ�.
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

    static bool s_binitialize_resource;    //���ҽ��� �ѹ��� �ҷ��;� �ϱ⿡ ���������� ǥ���մϴ�.

};

//���� Ŭ���� �������� �Դϴ�.
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