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

    /** Connection type.
     */
    enum class E_REPLICATED_RELIABLE : IZ_UINT32 {
        Reliable,
        UnReliable,
    };

    /** Base class for replicated property.
     */
    class ReplicatedPropertyBase {
        friend class ReplicatedObjectBase;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;

    protected:
        ReplicatedPropertyBase();
        virtual ~ReplicatedPropertyBase() {}

    protected:
        // Get if this is dirty.
        inline IZ_BOOL isDirty() const;

        // Set dirty forcibly.
        inline void dirty();

        // Set un-dirty forcibly.
        inline void unDirty();

        inline CStdList<ReplicatedPropertyBase>::Item* getListItem();

        PURE_VIRTUAL(IZ_CHAR* sync(IZ_CHAR* ptr));

    private:
        IZ_BOOL m_isDirty;

        CStdList<ReplicatedPropertyBase>::Item m_listItem;
    };

    /** Replicated property.
     */
    template <typename _T>
    class ReplicatedProperty : public ReplicatedPropertyBase {
    public:
        using DelegateNotify = std::function<void(void)>;

    public:
        ReplicatedProperty(
            ReplicatedObjectBase* obj,
            DelegateNotify notify,
            E_REPLICATED_TYPE type = E_REPLICATED_TYPE::None,
            E_REPLICATED_RELIABLE reliable = E_REPLICATED_RELIABLE::UnReliable)
        {
            m_owner = obj;

            if (type != E_REPLICATED_TYPE::None) {
                obj->addReplicatedProperty(*this);
            }

            m_type = type;
            m_realiable = reliable;
            m_funcNotify = notify;

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
        operator _T()
        {
            return (_T)m_value;
        }

        IZ_BOOL isReliable() const
        {
            return isReliable;
        }

    protected:
        // Get if this is on server.
        inline IZ_BOOL isServer();

        // Set value if this is on server.
        void set(const _T& rhs);

        // Set value forcibly.
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

#define IZ_REPLICATED_PROPERTY(clazz, type, prop, replicatedType, reliable) \
    void prop##_Notify(); \
    izanagi::net::ReplicatedProperty<type> prop{this, std::bind(&clazz::prop##_Notify, this), replicatedType, reliable}

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_H__)
