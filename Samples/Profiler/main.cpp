#include <windows.h>

#include "izStd.h"
#include "izSystem.h"

class CSampleTimer : public izanagi::IStdProfilerTimer
{
public:
    CSampleTimer() {}
    virtual ~CSampleTimer() {}

    virtual void Begin()
    {
        return m_Timer.Begin();
    }

    virtual IZ_FLOAT GetElapsedTime()
    {
        return m_Timer.End();
    }

private:
    izanagi::sys::CTimer m_Timer;
};

IZ_UINT8 buffer[1 * 1024 * 1024];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CStandardMemoryAllocator allocator;
    allocator.Init(sizeof(buffer), buffer);

    CSampleTimer timer;

    izanagi::CStdProfiler::Init(
        &allocator,
        &timer,
        IZ_NULL);

    izanagi::CStdProfiler::Enable(IZ_TRUE);

    izanagi::CStdProfiler::Begin("1");

    for (IZ_UINT i = 0; i < 10; i++) {

        izanagi::CStdProfiler::Begin("2");

        for (IZ_UINT n = 0; n < 10; n++) {

            izanagi::CStdProfiler::Begin("3");

            for (IZ_UINT x = 0; x < 10; x++) {

            }

            izanagi::CStdProfiler::End();
        }

        izanagi::CStdProfiler::End();
    }

    izanagi::CStdProfiler::End();

    izanagi::CStdProfiler::Terminate();

    allocator.Dump();

    return 0;
}
