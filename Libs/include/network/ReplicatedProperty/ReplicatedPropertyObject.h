#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__

#include <functional>
#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedProperty/ReplicatedPropertyManager.h"

namespace izanagi {
namespace net {
    class ReplicatedPropertyBase;

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

    protected:
        // Add replicated property to the object.
        void addReplicatedProperty(ReplicatedPropertyBase& prop);

        // Get id.
        IZ_UINT64 getReplicatedObjectID() const;

        // Set id.
        void setReplicatedObjectID(IZ_UINT64 id);

        // Get if this has dirty replicated property.
        IZ_BOOL hasDirtyReplicatedProperty() const;

        // Set this has dirty replicated properties.
        void dirtyReplicatedProperty();

        // Reset this has dirty replicated properties.
        void undirtyReplicatedProperty();

        CStdList<ReplicatedPropertyBase>::Item* getReplicatedPropertyListTopItem();

        CStdList<ReplicatedObjectBase>::Item* getListItem();

        CStdList<ReplicatedObjectBase>::Item* getListItemEx();

    private:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;

        // flag if this has dirty replicated property.
        std::atomic<IZ_BOOL> m_hasDirtyReplicatedProperty;

        // replicated property list.
        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;

        CStdList<ReplicatedObjectBase>::Item m_listItem;

        CStdList<ReplicatedObjectBase>::Item m_listItemEx;
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
        static izanagi::net::ReplicatedPropertyManager::ObjectCreatorBase* getCreator_##clazz() {\
            static izanagi::net::ReplicatedPropertyManager::ObjectCreator<clazz> __creator; \
            return &__creator;\
        }

#define IZ_GET_REPLICATED_OBJ_CLASS(clazz) clazz::Class##clazz()

#define IZ_REGIST_REPLICATED_OBJ(clazz)\
    izanagi::net::ReplicatedPropertyManager::get()->registerCreator(clazz::Class##clazz(), clazz::getCreator_##clazz())

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
