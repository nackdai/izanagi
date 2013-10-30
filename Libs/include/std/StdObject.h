#if !defined(__IZANAGI_STD_OBEJCT_H__)
#define __IZANAGI_STD_OBEJCT_H__

#include "izDefs.h"

namespace izanagi {
    class CObject {
    protected:
        CObject()
        {
            m_nRefCnt = 0;
        }
        virtual ~CObject() {}

        NO_COPIABLE(CObject);

    public:
        // 参照カウンタ増加
        void AddRef()
        {
            ++m_nRefCnt;
        }

        // 解放
        void Release()
        {
            if (m_nRefCnt > 0) {
                m_nRefCnt--;

                if (m_nRefCnt == 0) {
                    InternalRelease();
                }
            }
        }

        // 参照カウンタ取得
        IZ_INT GetRefCnt() const
        {
            return m_nRefCnt;
        }

    private:
        void* operator new(size_t size)
        {
            // Don't uset..
        }

    protected:
        void* operator new(size_t size, void* buf)
        {
            return buf;
        }

        void operator delete(void* data)
        {
            // Nothing is done.
        }

        void operator delete(void* data, void* buf)
        {
            // Nothing is done.
        }

    private:
        virtual void InternalRelease() = 0;

    private:
        // 参照カウンタ
        IZ_INT m_nRefCnt;
    };
}   // namespace izanagi

/**
* Utility for defining CObject::InternalRelease.
*/
#define IZ_DEFINE_INTERNAL_RELEASE()\
    void InternalRelease() {\
        delete this;\
        FREE(m_Allocator, this);\
    }

#endif  // #if !defined(__IZANAGI_STD_OBEJCT_H__)
