#ifndef _API_GAME_BITMAP_H_
#define _API_GAME_BITMAP_H_

#include "Global.h"
#include "Time.h"

class CBitmap_Base abstract{
public:

	virtual HDC				get_dc() = 0;
	virtual HBITMAP			get_bitmap() = 0;
	virtual const BITMAP&	get_data() const = 0;

};

class CBitmap : public CBitmap_Base{
public:
	CBitmap();
	CBitmap( const CBitmap& );
	CBitmap( CBitmap&& rvalue ) noexcept;
	~CBitmap() = default;

	//CBitmap_Base override
	virtual HDC				get_dc() override;
	virtual HBITMAP			get_bitmap() override;
	virtual const BITMAP&	get_data() const override;

	bool load( HWND hwindow, const TCHAR* sfile_path ); 

	void release();

	void flip_x();
	void flip_y();

	void flip_sprite_x( int islice );
	void flip_sprite_y( int islice );

private:

	HWND m_hwindow;

	HDC		m_hmemory_dc; 
	HBITMAP m_hold_bitmap;
	HBITMAP m_hbitmap; 

	BITMAP m_bitmap;

};

class CBidirection_Bitmap : public CBitmap_Base{
public:
	CBidirection_Bitmap();
	CBidirection_Bitmap( const CBidirection_Bitmap& lvalue );
	CBidirection_Bitmap( CBidirection_Bitmap&& rvalue ) noexcept;
	~CBidirection_Bitmap();

	bool load( HWND hwindow, const TCHAR* sfile_path, int islice );

	//CBitmap_Base override
	virtual HDC				get_dc() override;
	virtual HBITMAP			get_bitmap() override;
	virtual const BITMAP&	get_data() const override;

	void choice_origin();
	void choice_flip();

private:

	CBitmap* m_porigin_bitmap;
	CBitmap* m_pflip_x_bitmap;

	CBitmap* m_pselect_bitmap;

};

struct SPRITE{

	XY size;
	XY origin;
	XY step;

	XY frame_range_begin;
	XY frame_range_end;

};

class CAnimator{
public:

	enum class EAnimation_Mode{
		Normal,
		Inverse,
		Cycle,
		Element_Number
	};

	enum class EAddress_Mode{
		Wrap,
		Clamp,
		Element_Number
	};

public:
	CAnimator();
	CAnimator( const CAnimator& ) = default;
	CAnimator( CAnimator&& ) = default;
	~CAnimator() = default;
	CAnimator& operator=( const CAnimator& ) = default;
	CAnimator& operator=( CAnimator&& ) = default;

	void update();
	void inverse_update();
	void update_y();
	void inverse_update_y();

	void next_x();
	void back_x();
	void next_y();
	void back_y();

	void render( HDC hdc, const RECT& rect, COLORREF color_key );

	RECT calc_bitmap_rect() const;


	void set_bitmap( CBitmap_Base* pbitmap );
	void set_sprite( const SPRITE* psprite );

	void set_frame( const XY& frame );
	void set_animation_tick_ms( DWORD dwanimation_tick_ms );



	CBitmap_Base*	get_bitmap();
	const SPRITE*	get_sprite() const;

	const XY& get_frame() const;
	DWORD     get_animation_tick_ms() const;



private:

	void addressing();

	CBitmap_Base*	m_pbitmap;
	const SPRITE*	m_psprite;

	XY				m_frame;
	Time::CTimer	m_animation_tick_timer;
	DWORD			m_dwanimation_tick_ms;

	int m_bdirection;

};

class CBitmap_Manager{
	SINGLETON_DEFAULT( CBitmap_Manager );
public:

	typedef std::map< const TCHAR*, CBitmap_Base* > CBitmapPTR_Map;
	typedef std::map< const TCHAR*, SPRITE >		CSpritePTR_Map;

public:

	//Bitmap
	CBitmap_Base*		 inset_bitmap( HWND hwindow, const TCHAR* skey, const TCHAR* sfile_path );
	CBidirection_Bitmap* inset_bidirection_bitmap( HWND hwindow, const TCHAR* skey, const TCHAR* sfile_path, int islice );

	CBitmap_Base* find_bitmap( const TCHAR* skey );

	//Sprite
	bool insert_sprite( const TCHAR* skey, const SPRITE& sprite );

	const SPRITE* find_sprite( const TCHAR* skey );

	void release(); 

private:

	CBitmapPTR_Map m_pbitmap_map;
	CSpritePTR_Map m_psprite_map;

};

#define BITMAP_MANAGER Library::CSingleton< CBitmap_Manager >::get()

#endif