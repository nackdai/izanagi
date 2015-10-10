#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__

#include <functional>
#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedProperty/ReplicatedPropertyManager.h"

namespace izanagi {
namespace net {
    class ReplicatedPropertyBase;

    /**
     */
    class CClass {
    public:
        static CClass Invalid;

        static IZ_BOOL isValid(const CClass& clazz);

    public:
        CClass() : m_name(nullptr) {}
        CClass(const IZ_CHAR* tag);
        ~CClass() {}

        CClass(const CClass& rhs);
        const CClass& operator=(const CClass& rhs);

    public:
        IZ_BOOL operator==(const CClass& rhs) const;

        IZ_BOOL operator!=(const CClass& rhs) const;

        IZ_BOOL is(const CClass& clazz);

        operator const CKey&() const;

    private:
        CKey m_key;
        IZ_CHAR* m_name;
    };

    /** Porxy for ReplicatedPropertyManager.
     */
    class ReplicatedObjectProxy {
    public:
        ReplicatedObjectProxy(const CClass& clazz, ReplicatedPropertyManager::ReplicatedObjectCreator func)
        {
            izanagi::net::ReplicatedPropertyManager::get()->registerCreator(clazz, func);
        }
        ~ReplicatedObjectProxy() {}

        NO_COPIABLE(ReplicatedObjectProxy);
    };

    /** Base class for an object which has replicated properties.
     */
    class ReplicatedObjectBase {
        friend class ReplicatedPropertyManager;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;
        template <typename _T> friend class ReplicatedProperty;

    protected:
        ReplicatedObjectBase();
        virtual ~ReplicatedObjectBase();

    private:
        // Add replicated property to the object.
        void addReplicatedProperty(ReplicatedPropertyBase& prop);

        ReplicatedPropertyManager::HashItem* getReplicatedObjectHashItem();

    public:
        // Get if this is on server.
        IZ_BOOL isServer() const
        {
            return m_isServer;
        }

        virtual const CClass& getClass() = 0;

    private:
        // Get id.
        inline IZ_UINT64 getReplicatedObjectID() const;

        // Get if this has dirty replicated property.
        inline IZ_BOOL hasDirtyReplicatedProperty() const;

        inline void dirtyReplicatedProperty();
        inline void undirtyReplicatedProperty();

        inline CStdList<ReplicatedPropertyBase>::Item* getReplicatedPropertyListTopItem();

    private:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;

        IZ_BOOL m_hasReplicatedPropertyDirty;

        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;
        ReplicatedPropertyManager::HashItem m_ReplicatedObjectHashItem;
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
        static const izanagi::net::CClass& Class##clazz() {\
            static izanagi::net::CClass _class(#clazz);\
            return _class;\
        }\
        virtual const izanagi::net::CClass& getClass() override { return clazz::Class##clazz(); }\
        static izanagi::net::ReplicatedObjectBase* createForReplicatedProp##clazz(izanagi::IMemoryAllocator* allocator)\
        {\
            void* p = ALLOC(allocator, sizeof(clazz));\
            clazz* ret = new(p)clazz;\
            return ret;\
        }\
        izanagi::net::ReplicatedObjectProxy __pfoxy{ Class##clazz(), createForReplicatedProp##clazz };

#define IZ_GET_REPLICATED_OBJ_CLASS(clazz) clazz::Class##clazz()

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
