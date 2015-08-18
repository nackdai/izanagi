#include "network/ReplicatiedProperty.h"

namespace izanagi {
namespace net {

    class Hoge {
    public:
        Hoge() {}
        ~Hoge() {}

        IZ_BOOL Func(IZ_UINT32 v)
        {
            return IZ_TRUE;
        }
    };

    IZ_BOOL Func2(IZ_UINT32 v)
    {
        return IZ_TRUE;
    }

    void Func3()
    {

    }

    void Test()
    {
        Hoge hoge;
        //ReplicatedProperty<IZ_UINT32> v0(10, E_REPLICATED_TYPE::Rep, IZ_TRUE, std::bind(&Hoge::Func, &hoge, std::placeholders::_1));
        //ReplicatedProperty<IZ_UINT32> v0(10, E_REPLICATED_TYPE::Rep, IZ_TRUE, ReplicatedProperty<IZ_UINT32>::func(&Hoge::Func, &hoge));
        ReplicatedProperty<IZ_UINT32> v0(
            10, 
            E_REPLICATED_TYPE::Rep, 
            IZ_TRUE, 
            ReplicatedProperty<IZ_UINT32>::notify(&Func3),
            ReplicatedProperty<IZ_UINT32>::validation(&Func2));
        v0 = 1.0f;
        IZ_FLOAT x = v0;
    }
    
    template <typename _T>
    void ReplicatedProperty<_T>::set(const _T& rhs)
    {
        if (m_value != rhs) {
            IZ_BOOL isValid = IZ_TRUE;

            if (m_funcValidation
                && m_type != E_REPLICATED_TYPE::None)
            {
                isValid = m_funcValidation(rhs);
            }

            if (isValid) {
                m_value = rhs;

                // TODO
                // í êM
                if (m_type != E_REPLICATED_TYPE::None) {
                }

                // í ím
                if (m_funcNotify
                    && m_type == E_REPLICATED_TYPE::RepNotify)
                {
                    m_funcNotify();
                }
            }
        }
    }
}    // namespace net
}    // namespace izanagi
