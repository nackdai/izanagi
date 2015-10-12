#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__

#include <functional>
#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedProperty/ReplicatedPropertyManager.h"

namespace izanagi {
namespace net {
    class ReplicatedPropertyBase;

    /** Porxy for ReplicatedPropertyManager.
     */
    template <typename TYPE>
    class ReplicatedObjectProxy {
    public:
        ReplicatedObjectProxy(const ReplicatedObjectClass& clazz)
        {
            // Regist creator which creates specified object.
            izanagi::net::ReplicatedPropertyManager::get()->registerCreator(clazz, &m_creator);
        }
        ~ReplicatedObjectProxy() {}

        NO_COPIABLE(ReplicatedObjectProxy);

    private:
        ReplicatedPropertyManager::ObjectCreator<TYPE> m_creator;
    };

    /** Base class for an object which has replicated properties.
     */
    class ReplicatedObjectBase : public CPlacementNew {
        friend class ReplicatedPropertyManager;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;
        template <typename _T> friend class ReplicatedProperty;

    protected:
        ReplicatedObjectBase();
        virtual ~ReplicatedObjectBase();

    public:
        // Get if this is on server.
        IZ_BOOL isServer() const
        {
            return m_isServer;
        }

        virtual const ReplicatedObjectClass& getClass() = 0;

    private:
        // Add replicated property to the object.
        inline void addReplicatedProperty(ReplicatedPropertyBase& prop);

        // Get id.
        inline IZ_UINT64 getReplicatedObjectID() const;

        // Get if this has dirty replicated property.
        inline IZ_BOOL hasDirtyReplicatedProperty() const;

        // Set this has dirty replicated properties.
        inline void dirtyReplicatedProperty();

        // Reset this has dirty replicated properties.
        inline void undirtyReplicatedProperty();

        inline CStdList<ReplicatedPropertyBase>::Item* getReplicatedPropertyListTopItem();

        inline CStdList<ReplicatedObjectBase>::Item* getListItem();

    private:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;

        // flag if this has dirty replicated property.
        std::atomic<IZ_BOOL> m_hasDirtyReplicatedProperty;

        // replicated property list.
        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;

        CStdList<ReplicatedObjectBase>::Item m_listItem;
    };

    /** Replicated Object.
     */
    template <class BASE = NullClass>
    class ReplicatedObject : public ReplicatedObjectBase, BASE {
    protected:
        ReplicatedObject() {}
        virtual ~ReplicatedObject() {}
    };
}    // namespace net
}    // namespace izanagi

#define IZ_DEFS_REPLICATED_OBJ(clazz) \
    public:\
        static const izanagi::net::ReplicatedObjectClass& Class##clazz() {\
            static izanagi::net::ReplicatedObjectClass _class(#clazz);\
            return _class;\
        }\
        virtual const izanagi::net::ReplicatedObjectClass& getClass() override { return clazz::Class##clazz(); }\
    private:\
        static izanagi::net::ReplicatedObjectProxy<clazz> pfoxy##clazz;

#define IZ_DECL_REPLICATED_OBJ(clazz) \
        izanagi::net::ReplicatedObjectProxy<clazz> clazz::pfoxy##clazz{ clazz::Class##clazz() }

#define IZ_GET_REPLICATED_OBJ_CLASS(clazz) clazz::Class##clazz()

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
