#include "izSampleKit.h"
#include "izSystem.h"
#include "izThreadModel.h"

#define MSG_MAX (10)

izanagi::threadmodel::CMessageQueue queue;
IZ_BOOL isFinish = IZ_FALSE;

class CTestThread : public izanagi::sys::CThread
{
public:
    CTestThread() {}
    virtual ~CTestThread() {}

public:
    virtual izanagi::threadmodel::CMessage* GetMessage(IZ_UINT pos) = 0;

    virtual void Run()
    {
        izanagi::threadmodel::CMessage* msg = IZ_NULL;

        IZ_UINT cnt = 0;

        while (IZ_TRUE)
        {
            msg = GetMessage(cnt);
            if (msg != IZ_NULL)
            {
                cnt++;
                izanagi::CValue value;
                msg->GetValueProxy(value);
                IZ_PRINTF("%d\n", value.GetValueAsUInt32());

                if (cnt >= MSG_MAX)
                {
                    break;
                }
            }
            else
            {
                if (isFinish)
                {
                    break;
                }
            }

            izanagi::sys::CThread::Sleep(100);
        }
    }
};

class CTestThread_Peek : public CTestThread
{
public:
    CTestThread_Peek() {}
    virtual ~CTestThread_Peek() {}

public:
    izanagi::threadmodel::CMessage* GetMessage(IZ_UINT pos)
    {
        return queue.Peek(0);
    }
};

class CTestThread_Get : public CTestThread
{
public:
    CTestThread_Get() {}
    virtual ~CTestThread_Get() {}

public:
    izanagi::threadmodel::CMessage* GetMessage(IZ_UINT pos)
    {
        return queue.Get(0);
    }
};

class CTestThread_PeekWithMatchFull : public CTestThread
{
public:
    CTestThread_PeekWithMatchFull() {}
    virtual ~CTestThread_PeekWithMatchFull() {}

public:
    izanagi::threadmodel::CMessage* GetMessage(IZ_UINT pos)
    {
        return queue.PeekWithMatchFull(pos);
    }
};

class CTestThread_GetWithMatchFull : public CTestThread
{
public:
    CTestThread_GetWithMatchFull() {}
    virtual ~CTestThread_GetWithMatchFull() {}

public:
    izanagi::threadmodel::CMessage* GetMessage(IZ_UINT pos)
    {
        return queue.GetWithMatchFull(pos);
    }
};

class CTestMessage : public izanagi::threadmodel::CMessageImpl<IZ_UINT>
{
public:
    CTestMessage() {}
    virtual ~CTestMessage() {}

public:
    virtual IZ_BOOL GetValueProxy(izanagi::CValue& ret)
    {
        ret.SetValue(GetValue());
        return IZ_TRUE;
    }
};

template <typename _T>
void Test(IZ_BOOL isPosToZero)
{
    isFinish = IZ_FALSE;

    CTestMessage message[MSG_MAX];

    _T thread;

    thread.Start();

    izanagi::sys::CThread::Sleep(1000);

    for (IZ_UINT i = 0; i < MSG_MAX; i++)
    {
        message[i].SetValue(i);

        if (isPosToZero)
        {
            queue.Post(0, &message[i]);
        }
        else
        {
            queue.Post(i, &message[i]);
        }

        izanagi::sys::CThread::YieldThread();
    }

    isFinish = IZ_TRUE;

    thread.Join();
}

int TestMessageQueue(izanagi::IMemoryAllocator* allocator)
{
    Test<CTestThread_Peek>(IZ_TRUE);
    Test<CTestThread_Get>(IZ_TRUE);
    Test<CTestThread_PeekWithMatchFull>(IZ_FALSE);
    Test<CTestThread_GetWithMatchFull>(IZ_FALSE);

    return 0;
}
