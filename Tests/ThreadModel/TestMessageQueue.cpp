#include "izSampleKit.h"
#include "izSystem.h"
#include "izThreadModel.h"

izanagi::threadmodel::CMessageQueue queue;
IZ_BOOL isFinish = IZ_FALSE;

class CTestThread : public izanagi::CThread
{
public:
    CTestThread() {}
    virtual ~CTestThread() {}

public:
    virtual void Run()
    {
        izanagi::threadmodel::CMessage* msg = IZ_NULL;

        IZ_UINT cnt = 0;

        while (IZ_TRUE)
        {
            //msg = queue.Peek(0);
            //msg = queue.Get(0);
            //msg = queue.PeekWithMatchFull(cnt);
            msg = queue.GetWithMatchFull(cnt);
            if (msg != IZ_NULL)
            {
                cnt++;
                izanagi::CValue value;
                msg->GetValueProxy(value);
                IZ_PRINTF("%d\n", value.GetValueAsUInt32());
            }
            else
            {
                if (isFinish)
                {
                    break;
                }
            }

            izanagi::CThread::Sleep(100);
        }
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

int TestMessageQueue(izanagi::IMemoryAllocator* allocator)
{
    CTestMessage message[10];

    CTestThread thread;

    thread.Start();

    izanagi::CThread::Sleep(1000);

    for (IZ_UINT i = 0; i < 10; i++)
    {
        message[i].SetValue(i);
        //queue.Post(0, &message[i]);
        queue.Post(i, &message[i]);

        izanagi::CThread::YieldThread();
    }

    isFinish = IZ_TRUE;

    thread.Join();

    return 0;
}
