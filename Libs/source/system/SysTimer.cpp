#include <windows.h>
#include <mmsystem.h>
#include "system/SysTimer.h"

using namespace uranus;

/**
* コンストラクタ
*/
CTimer::CTimer()
{
	LARGE_INTEGER sFreq;
	FILL_ZERO(&sFreq, sizeof(sFreq));
	
	m_fTime = 0.0f;

	UN_BOOL result = QueryPerformanceFrequency(&sFreq);
	UN_ASSERT(result);

	m_Frequency = sFreq.QuadPart;
}

/**
* 計測開始
*/
void CTimer::Begin()
{
	LARGE_INTEGER sBegin;
	FILL_ZERO(&sBegin, sizeof(sBegin));

	UN_BOOL result = QueryPerformanceCounter(&sBegin);
	UN_ASSERT(result);

	m_Begin = sBegin.QuadPart;
}

/**
* 計測終了
*/
UN_FLOAT CTimer::End()
{
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);

	m_fTime = (cur.QuadPart - m_Begin) * 1000.0f / m_Frequency;
	return m_fTime;
}
