#if !defined(_IZANAGI_NETWORK_NETWORK_CALLBACK_REGISTER_H__)
#define _IZANAGI_NETWORK_NETWORK_CALLBACK_REGISTER_H__

#include <functional>
#include <atomic>
#include <uv.h>
#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
namespace net {
    class CalbackBase;

    class CallbackRegister {
    private:
        CallbackRegister();
        ~CallbackRegister();

    public:
        enum Type : IZ_UINT {
            Connect,
            Listen,
            Alloc,
            Read,
            Write,
            Close,
            Recieve,
        };

        class Key {
        public:
            Key() {}
            Key(Type type, void* data)
            {
                m_type = type;
                m_data = data;
            }
            ~Key() {}

            IZ_BOOL operator==(const Key& rhs)
            {
                return (m_type == rhs.m_type && m_data == rhs.m_data);
            }

            operator IZ_UINT() const
            {
                return (IZ_UINT)m_type;
            }

        private:
            Type m_type;
            void* m_data;
        };

        using HashItem = CStdHash<Key, CalbackBase, 4>::Item;

        static void Regist(CalbackBase& callback);

        static void RegistPermanently(CalbackBase& callback);

        static void Remove(CalbackBase& callback);

        template <typename CB, typename ...ARGS>
        static void Invoke(const Key& key, ARGS&& ... args)
        {
            auto ptr = s_mapCallback.FindData(key);
            IZ_ASSERT(ptr != nullptr);

            Callback<CB>* cb = reinterpret_cast<Callback<CB>*>(ptr);
            cb->Invoke(std::forward<ARGS>(args)...);

            if (!cb->IsPermanently()) {
                ptr->GetHashItem()->Leave();
            }
        }

    private:
        static CStdHash<Key, CalbackBase, 4> s_mapCallback;
    };

    class CalbackBase {
        friend class CallbackRegister;

    protected:
        CalbackBase() {}
        virtual ~CalbackBase() {}

    protected:
        inline CallbackRegister::HashItem* GetHashItem()
        {
            return &m_hashItem;
        }

        inline void EnablePermanently()
        {
            m_isPermanently = IZ_TRUE;
        }

        inline IZ_BOOL IsPermanently() const
        {
            return m_isPermanently;
        }

    private:
        CallbackRegister::HashItem m_hashItem;
        IZ_BOOL m_isPermanently{ IZ_FALSE };
    };

    template <typename CB>
    class Callback : public CalbackBase {
    public:
        Callback() {}
        virtual ~Callback() {}

        Callback(void* key, CB&& callback)
        {
            Set(key, callback);
        }

    public:
        void Set(CallbackRegister::Key key, CB&& callback)
        {
            GetHashItem()->Init(key, this);
            m_callback = std::forward<CB>(callback);
        }

        template <typename ...ARGS>
        void Invoke(ARGS&& ... args)
        {
            IZ_ASSERT(m_callback);
            m_callback(std::forward<ARGS>(args)...);
        }

    private:
        CB m_callback{ nullptr };
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_NETWORK_TCP_LIBUV_H__)
