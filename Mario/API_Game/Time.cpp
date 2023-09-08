#include "Time.h"

#include "Graphics.h"

API_GAME_TIME_NAMESPACE_USING

CTimer::CTimer() : m_frequency{},
                   m_old_time{}{
    QueryPerformanceFrequency( &m_frequency );
    QueryPerformanceCounter( &m_old_time );
}

bool CTimer::signal( __int64 itime_ms, bool bauto_reset ){

    LARGE_INTEGER current_time;
    QueryPerformanceCounter( &current_time );

    __int64 idelta_per_ms = ( ( current_time.QuadPart - m_old_time.QuadPart ) * 1000 ) / m_frequency.QuadPart;
    if( idelta_per_ms > itime_ms ){
        if( bauto_reset ) m_old_time = current_time;
        return true;
    }

    return false;

}

DWORD CTimer::get_delta_time_ms() const{

    LARGE_INTEGER current_time;
    QueryPerformanceCounter( &current_time );

    return ( DWORD )( ( ( current_time.QuadPart - m_old_time.QuadPart ) * 1000 ) / m_frequency.QuadPart );

}

float CTimer::get_delta_time_normalization() const{

    LARGE_INTEGER current_time;
    QueryPerformanceCounter( &current_time );

    return ( float )( current_time.QuadPart - m_old_time.QuadPart ) / ( float )m_frequency.QuadPart;

}

void CTimer::reset(){
    QueryPerformanceCounter( &m_old_time );
}

bool CTime_Manager::initialize(){
    m_tick_timer.reset();
    return true;
}

void CTime_Manager::release(){
}

void CTime_Manager::update(){

    m_ftick = m_tick_timer.get_delta_time_normalization();
    m_ftick = min( m_ftick, 1.0f / 60.0f );

    m_tick_timer.reset();

}

float CTime_Manager::get_tick() const{
    return m_ftick;
}