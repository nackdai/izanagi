#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_H__

#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedPropertyObject.h"

namespace izanagi {
namespace net {
    /**
     */
    enum class E_REPLICATED_TYPE : IZ_UINT32 {
        Rep,
        RepNotify,
        None,
    };

    /**
     */
    class ReplicatedPropertyBase {
        friend class ReplicatedObjectBase;

    protected:
        ReplicatedPropertyBase() {}
        virtual ~ReplicatedPropertyBase() {}

    private:
        CStdList<ReplicatedPropertyBase>::Item* getListItem();

    private:
        CStdList<ReplicatedPropertyBase>::Item m_listItem;
    };

    /**
     */
    template <typename _T>
    class ReplicatedProperty : public ReplicatedPropertyBase {
        friend class ReplicatedPropertyManager;

    public:
        typedef std::function<IZ_BOOL(_T)> DelegateValidation;
        typedef std::function<void(void)> DelegateNotify;

        // NOTE
        // http://qiita.com/tkyaji/items/1aa7bb01658e848d3ef4

        template <class _O>
        static DelegateValidation validation(IZ_BOOL(_O::*f)(_T), _O* obj)
        {
            return std::bind(f, obj, std::placeholders::_1);
        }

        static DelegateValidation validation(IZ_BOOL(*f)(_T))
        {
            return std::bind(f, std::placeholders::_1);
        }

        template <class _O>
        static DelegateNotify notify(void(_O::* f)(void), _O* obj)
        {
            return std::bind(f, obj);
        }

        static DelegateNotify notify(void(*f)(void))
        {
            return std::bind(f);
        }

    public:
        ReplicatedProperty(
            ReplicatedObjectBase* obj,
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            IZ_BOOL isReliable = IZ_FALSE,
            DelegateNotify funcNotify = nullptr,
            DelegateValidation funcValidation = nullptr)
        {
            obj->addReplicatedProperty(*this);

            m_type = type;
            m_funcNotify = funcNotify;
            m_funcValidation = funcValidation;
            m_isReliable = isReliable;

            // NOTE
            // 通常の変数と同様に明示的に初期化されない場合のm_valueの値は不定.
        }

        ReplicatedProperty(
            const _T& rhs,
            ReplicatedObjectBase* obj,
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            IZ_BOOL isReliable = IZ_FALSE,
            DelegateNotify funcNotify = nullptr,
            DelegateValidation funcValidation = nullptr)
        {
            obj->addReplicatedProperty(*this);

            m_type = type;
            m_funcNotify = funcNotify;
            m_funcValidation = funcValidation;
            m_isReliable = isReliable;

            set(rhs);
        }

        virtual ~ReplicatedProperty()
        {
        }

        const ReplicatedProperty& operator=(const _T& rhs)
        {
            set(rhs);
            return *this;
        }

        NO_COPIABLE(ReplicatedProperty);

    public:
        operator _T()
        {
            return (_T)m_value;
        }

        IZ_BOOL isReliable() const
        {
            return isReliable;
        }

    protected:
        void set(const _T& rhs);

    protected:
        E_REPLICATED_TYPE m_type;

        _T m_value;

        DelegateNotify m_funcNotify;
        DelegateValidation m_funcValidation;

        IZ_BOOL m_isReliable;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
