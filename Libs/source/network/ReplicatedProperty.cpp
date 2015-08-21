#include "network/ReplicatedProperty.h"
#include "network/ReplicatedPropertyObject.h"

namespace izanagi {
namespace net {
    ReplicatedPropertyBase::ReplicatedPropertyBase()
    {
        m_isDirty = IZ_FALSE;
        m_listItem.Init(this);
    }

    void ReplicatedPropertyBase::dirty()
    {
        m_isDirty = IZ_TRUE;
    }

    void ReplicatedPropertyBase::unDirty()
    {
        m_isDirty = IZ_FALSE;
    }

    //////////////////////////////////////////////

    template <typename _T>
    IZ_BOOL ReplicatedProperty<_T>::isServer()
    {
        return m_owner->isServer();
    }

    template <typename _T>
    void ReplicatedProperty<_T>::set(const _T& rhs)
    {
        // TODO
        // クライアント側では値の変更を許さない？
        if (!isServer()) {
            return;
        }

        setForcibly(rhs);
    }

    template <typename _T>
    void ReplicatedProperty<_T>::setForcibly(const _T& rhs)
    {
        if (m_value != rhs) {
            // NOTE
            // dirtyにならない限り、通信対象にならない
            if (m_type != E_REPLICATED_TYPE::None
                && isServer())
            {
                // TODO
                // サーバーで呼ばれたときだけdirtyにする？

                this->dirty();
                m_owner->dirtyReplicatedProperty();
            }

            m_value = rhs;

            // 通知
            if (m_funcNotify
                && m_type == E_REPLICATED_TYPE::RepNotify
                && !isServer())
            {
                // クライアントで呼ばれたときは通知する
                m_funcNotify();
            }
        }
    }

    template <typename _T>
    IZ_CHAR* ReplicatedProperty<_T>::sync(IZ_CHAR* ptr)
    {
        _T value = (_T)*ptr;

        setForcibly(value);

        ptr += sizeof(_T);

        return ptr;
    }

    //////////////////////////////////////////////

    class Hoge : public ReplicatedObject<> {
        IZ_DEFS_REPLICATED_OBJ(Hoge);

    public:
        Hoge()
        {
            IZ_DECL_REPLICATED_PROPERTY(Hoge, v0);
        }
        ~Hoge() {}

        void Func()
        {
        }

        IZ_REPLICATED_PROPERTY(IZ_UINT32, v0, E_REPLICATED_TYPE::Rep, E_REPLICATED_RELIABLE::Reliable);
    };

    IZ_REFLECT_REPLICATED_OBJ(Hoge);

    void Test()
    {
        Hoge hoge;
        hoge.v0 = 1.0f;
        IZ_FLOAT x = hoge.v0;
    }
}    // namespace net
}    // namespace izanagi
