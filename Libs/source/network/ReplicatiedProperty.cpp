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

    void Test()
    {
        Hoge hoge;
        ReplicatedProperty::Delegate d(&hoge, &Hoge::Func);
        //ReplicatedProperty<IZ_UINT32> v0(10, &hoge, &Hoge::Func);
        //v0 = 1.0f;
        //IZ_FLOAT x = v0;
    }
#if 0
    template <typename _T>
    void ReplicatedProperty<_T>::set(const _T& rhs)
    {
        if (m_value != rhs) {
            IZ_BOOL isValid = IZ_TRUE;

            if (m_flags.withValidation) {
                // TODO
            }

            if (isValid) {
                m_value = rhs;

                // TODO
                // í êM
            }
        }
    }
#endif
}    // namespace net
}    // namespace izanagi
