#if !defined(__IZANAGI_STD_OBEJCT_H__)
#define __IZANAGI_STD_OBEJCT_H__

#include "izDefs.h"

namespace izanagi {
    class ObjectReference;

    /**
     */
    class CObject
    {
        friend class ObjectReference;

        struct Callback {
            void operator()()
            {
                (reference->*func)();
            }

            void Remove()
            {
                if (prev) {
                    prev->next = next;
                }
                if (next) {
                    next->prev = prev;
                }

                prev = next = IZ_NULL;
            }

            typedef void (ObjectReference::* Func)();

            ObjectReference* reference;
            Func func;

            Callback* prev;
            Callback* next;

            Callback()
            {
                prev = next = IZ_NULL;
            }

            ~Callback()
            {
                Remove();
            }
        };

    protected:
        CObject()
        {
            m_nRefCnt = 0;
            m_Callback = IZ_NULL;
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
                    if (m_Callback) {
                        Callback* callback = m_Callback;
                        while (callback) {
                            (*callback)();
                            Callback* next = callback->next;
                            callback->Remove();
                            callback = next;
                        }
                    }

                    InternalRelease();
                }
            }
        }

    private:
        void AddCallback(Callback* callback)
        {
            if (m_Callback) {
                m_Callback->prev = callback;
                callback->next = m_Callback;
            }
            m_Callback = callback;
        }

        void RemoveCallback(Callback* callback)
        {
            callback->Remove();

            if (m_Callback == callback) {
                m_Callback = m_Callback->next;
            }
        }

        void* operator new(size_t size) throw()
        {
            // Don't uset..
        }

    protected:
        void* operator new(size_t size, void* buf) throw()
        {
            return buf;
        }

        void operator delete(void* data) throw()
        {
            // Nothing is done.
        }

        void operator delete(void* data, void* buf) throw()
        {
            // Nothing is done.
        }

    private:
        virtual void InternalRelease() = 0;

    private:
        // 参照カウンタ
        IZ_INT m_nRefCnt;

        Callback* m_Callback;
    };

    /**
     */
    class ObjectReference
    {
    public:
        ObjectReference()
        {
            Init();
        }
        
        ObjectReference(CObject* obj)
        {
            Init();
            Set(obj);
        }

        ObjectReference(const ObjectReference& rhs)
        {
            Init();
            Set(rhs.m_Obj);
        }

        const ObjectReference& operator=(const ObjectReference& rhs)
        {
            Init();
            Set(rhs.m_Obj);
        }

        ~ObjectReference()
        {
        }

        void Init()
        {
            m_Callback.reference = this;
            m_Callback.func = &ObjectReference::ListenObjectReleased;
            m_Obj = IZ_NULL;
        }

    public:
        IZ_BOOL IsAlive() const
        {
            return (m_Obj != IZ_NULL);
        }

        CObject* GetTarget()
        {
            return m_Obj;
        }

        void Set(CObject* obj)
        {
            if (m_Obj) {
                m_Obj->RemoveCallback(&m_Callback);
            }

            m_Obj = obj;

            if (m_Obj) {
                m_Obj->AddCallback(&m_Callback);
            }
        }

    private:
        void ListenObjectReleased()
        {
            m_Obj = IZ_NULL;
        }

    private:
        CObject* m_Obj;

        CObject::Callback m_Callback;
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
