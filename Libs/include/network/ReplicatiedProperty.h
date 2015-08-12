#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
namespace net {
    /**
     */
    enum class E_REPLICATED_TYPE : IZ_UINT32 {
        Rep,
        RepNotify,
        None,

        NUM,
    };

    /**
     */
    template <typename _T>
    class ReplicatedProperty {
    public:
        template <class _O>
        class Delegate : public FuncDelegate<IZ_BOOL, _T>
        {
            typedef IZ_BOOL(_O::* FUNC)(_T);

        public:
            Delegate(_O* object, IZ_BOOL(_O::*func)(_T)) : FuncDelegate<IZ_BOOL, _T>(object, IZ_NULL), m_Func(func) {}
            virtual ~Delegate() {}

        private:
            Delegate() : FuncDelegate<IZ_BOOL, _T>(IZ_NULL, IZ_NULL), m_Func(IZ_NULL) {}
            Delegate(const Delegate& rhs) {}

        private:
            virtual IZ_BOOL Execute(_T arg1)
            {
                _O* object = (_O*)izanagi::Delegate::GetObject();
                IZ_ASSERT(object != IZ_NULL);

                IZ_ASSERT(m_Func != IZ_NULL);

                IZ_BOOL tmp = (object->*m_Func)(arg1);
                return tmp;
            }

        protected:
            const Delegate& operator = (const Delegate& rhs)
            {
                m_Object = rhs.m_Object;
                m_Func = rhs.m_Func;
            }

        private:
            FUNC m_Func;
        };

    public:
        ReplicatedProperty(
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            IZ_BOOL isReliable = IZ_FALSE)
        {
            // NOTE
            // 通常の変数と同様に明示的に初期化されない場合のm_valueの値は不定.

            m_type = type;
            m_flags.isReliable = isReliable;
        }

        virtual ~ReplicatedProperty()
        {
        }

        ReplicatedProperty(
            const ReplicatedProperty& rhs,
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            IZ_BOOL isReliable = IZ_FALSE)
        {
            m_type = type;
            m_flags.isReliable = isReliable;

            set(rhs.m_value);
        }

        ReplicatedProperty(
            const _T& rhs,
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            IZ_BOOL isReliable = IZ_FALSE)
        {
            m_type = type;
            m_flags.isReliable = isReliable;

            set(rhs);
        }

        const ReplicatedProperty& operator=(const ReplicatedProperty& rhs)
        {
            return *this = rhs.m_value;
        }

        const ReplicatedProperty& operator=(const _T& rhs)
        {
            set(rhs);
            return *this;
        }

    public:
        operator _T()
        {
            return (_T)m_value;
        }

        IZ_BOOL isReliable() const
        {
            return isReliable;
        }

        template <class _O>
        void setFuncValidation(Delegate<_T, _O, _F>& func) const
        {
            m_funcValidation = func;
        }

    protected:
        void set(const _T& rhs);

    protected:
        E_REPLICATED_TYPE m_type;
        _T m_value;

        Delegate<_T, _O, _F> m_funcValidation;

        struct {
            IZ_UINT isReliable      : 1;
        } m_flags;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
