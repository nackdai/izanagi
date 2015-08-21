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

    enum class E_REPLICATED_RELIABLE : IZ_UINT32 {
        Reliable,
        UnReliable,
    };

    /**
     */
    class ReplicatedPropertyBase {
        friend class ReplicatedObjectBase;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;

    protected:
        ReplicatedPropertyBase();
        virtual ~ReplicatedPropertyBase() {}

    protected:
        inline IZ_BOOL isDirty() const;

        inline void dirty();
        inline void unDirty();

        inline CStdList<ReplicatedPropertyBase>::Item* getListItem();

        PURE_VIRTUAL(IZ_CHAR* sync(IZ_CHAR* ptr));

    private:
        IZ_BOOL m_isDirty;

        CStdList<ReplicatedPropertyBase>::Item m_listItem;
    };

    /**
     */
    template <typename _T>
    class ReplicatedProperty : public ReplicatedPropertyBase {
    public:
        using DelegateNotify = std::function<void(void)>;

        // NOTE
        // http://qiita.com/tkyaji/items/1aa7bb01658e848d3ef4

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
            E_REPLICATED_RELIABLE reliable = E_REPLICATED_RELIABLE::UnReliable)
        {
            m_owner = obj;
            obj->addReplicatedProperty(*this);

            m_type = type;
            m_realiable = reliable;
            m_funcNotify = nullptr;

            // NOTE
            // 通常の変数と同様に明示的に初期化されない場合のm_valueの値は不定.
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
        template <class _O>
        void init(void(_O::* f)(void), _O* obj)
        {
            if (!m_funcNotify) {
                m_funcNotify = notify(f, obj);
            }
        }

        operator _T()
        {
            return (_T)m_value;
        }

        IZ_BOOL isReliable() const
        {
            return isReliable;
        }

    protected:
        inline IZ_BOOL isServer();

        void set(const _T& rhs);

        void setForcibly(const _T& rhs);

        virtual IZ_CHAR* sync(IZ_CHAR* ptr) override;

    protected:
        ReplicatedObjectBase* m_owner;

        E_REPLICATED_TYPE m_type;
        E_REPLICATED_RELIABLE m_realiable;

        _T m_value;

        DelegateNotify m_funcNotify;
    };
}    // namespace net
}    // namespace izanagi

#define IZ_REPLICATED_PROPERTY(type, prop, replicatedType, reliable) \
    void prop##_Notify(); \
    izanagi::net::ReplicatedProperty<type> prop{this, replicatedType, reliable}

#define IZ_DECL_REPLICATED_PROPERTY(clazz, prop) prop.init(& clazz::prop##_Notify, this);

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
