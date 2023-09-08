#ifndef _API_GAME_TIME_H_
#define _API_GAME_TIME_H_

#include "Global.h"

API_GAME_TIME_NAMESPACE_BEGIN

class CTimer{
public:
    CTimer();
    CTimer( const CTimer& ) = default;
    CTimer( CTimer&& ) = default;
    ~CTimer() = default;
	CTimer& operator=( const CTimer& ) = default;
	CTimer& operator=( CTimer&& ) = default;

    bool signal( __int64 itime_ms, bool bauto_reset = true );
    DWORD get_delta_time_ms() const;
    float get_delta_time_normalization() const;

    void reset();

private:

    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_old_time;

};

class CTime_Manager{
public:

    class Helper;

public:

    bool initialize();
    void release();

    void update();

    float get_tick() const;

private:
    SINGLETON_DEFAULT( CTime_Manager );

    CTimer m_tick_timer;
    float  m_ftick;

};

class CTime_Manager::Helper{
public:

};

#define TIME_MANAGER Library::CSingleton< Time::CTime_Manager >::get()

API_GAME_TIME_NAMESPACE_END

#endif