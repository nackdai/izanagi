#include <windows.h>

#include "izStd.h"

class SampleObject : public izanagi::CObject
{
public:
    static SampleObject* Create(izanagi::IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(SampleObject));

        SampleObject* ret = new(buf) SampleObject();
        {
            ret->AddRef();
            ret->m_Allocator = allocator;
        }

        return ret;
    }

private:
    SampleObject()
    {
    }

    virtual ~SampleObject()
    {
    }

    virtual void InternalRelease()
    {
        delete this;
        FREE(m_Allocator, this);
    }

private:
    izanagi::IMemoryAllocator* m_Allocator;
};

IzMain(0, 0)
{
    izanagi::CSimpleMemoryAllocator allocator;

    IZ_PRINTF("***** Simple reference sample *****\n");
    {
        SampleObject* obj = SampleObject::Create(&allocator);

        izanagi::ObjectReference ref(obj);

        if (ref.IsAlive()) {
            IZ_PRINTF("Alive\n");
        }

        SAFE_RELEASE(obj);

        if (!ref.IsAlive()) {
            IZ_PRINTF("Not Alive\n");
        }
    }

    IZ_PRINTF("***** Two reference sample *****\n");
    {
        SampleObject* obj = SampleObject::Create(&allocator);

        izanagi::ObjectReference ref0(obj);
        izanagi::ObjectReference ref1(obj);

        if (ref0.IsAlive()) {
            IZ_PRINTF("Ref0 Alive\n");
        }
        if (ref1.IsAlive()) {
            IZ_PRINTF("Ref1 Alive\n");
        }

        SAFE_RELEASE(obj);

        if (!ref0.IsAlive()) {
            IZ_PRINTF("Ref0 Not Alive\n");
        }
        if (!ref1.IsAlive()) {
            IZ_PRINTF("Ref1 Not Alive\n");
        }
    }

    IZ_PRINTF("***** Set new object sample *****\n");
    {
        SampleObject* obj0 = SampleObject::Create(&allocator);
        SampleObject* obj1 = SampleObject::Create(&allocator);

        izanagi::ObjectReference ref(obj0);

        if (ref.IsAlive()) {
            IZ_PRINTF("Alive\n");
        }

        ref.Set(obj1);

        SAFE_RELEASE(obj0);

        if (ref.IsAlive()) {
            IZ_PRINTF("Alive\n");
        }

        SAFE_RELEASE(obj1);

        if (!ref.IsAlive()) {
            IZ_PRINTF("Not Alive\n");
        }
    }

    return 0;
}
