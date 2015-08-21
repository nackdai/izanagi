#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__

#include "izDefs.h"
#include "izStd.h"
#include "network/ReplicatedPropertyManager.h"

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

        CClass(const CClass& rhs) = delete;
        const CClass& operator=(const CClass& rhs);

    public:
        IZ_BOOL operator==(const CClass& rhs) const;

        IZ_BOOL operator!=(const CClass& rhs) const;

        operator const CKey&() const;

    private:
        CKey m_key;
        IZ_CHAR* m_name;
    };

    /**
     */
    class ReplicatedObjectBase {
        friend class ReplicatedPropertyManager;
        friend class ReplicatedPropertyServer;
        friend class ReplicatedPropertyClient;
        template <typename _T> friend class ReplicatedProperty;

    protected:
        ReplicatedObjectBase();
        virtual ~ReplicatedObjectBase() {}

    private:
        void addReplicatedProperty(ReplicatedPropertyBase& prop);

        ReplicatedPropertyManager::HashItem* getReplicatedObjectHashItem();

    public:
        IZ_BOOL isServer() const
        {
            return m_isServer;
        }

        virtual const CClass& getClass() = 0;

    private:
        inline IZ_UINT64 getReplicatedObjectID() const;

        inline IZ_BOOL isDirtyReplicatedProperty() const;

        inline void dirtyReplicatedProperty();
        inline void undirtyReplicatedProperty();

        inline CStdList<ReplicatedPropertyBase>::Item* getReplicatedPropertyListTopItem();

    private:
        IZ_UINT64 m_ReplicatedObjectID;
        IZ_BOOL m_isServer;

        IZ_BOOL m_isReplicatedPropertyDirty;

        CStdList<ReplicatedPropertyBase> m_ReplicatedPropertyList;
        ReplicatedPropertyManager::HashItem m_ReplicatedObjectHashItem;
    };

    /**
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
        struct CInternalForReplicatedProp##clazz { \
                CInternalForReplicatedProp##clazz() { izanagi::net::ReplicatedPropertyManager::get()->registerCreator(Class##clazz(), createForReplicatedProp##clazz); }\
                ~CInternalForReplicatedProp##clazz() {}\
            };\
        static CInternalForReplicatedProp##clazz s_internalForReplicatedProp##clazz

#define IZ_REFLECT_REPLICATED_OBJ(clazz) \
    clazz::CInternalForReplicatedProp##clazz clazz::s_internalForReplicatedProp##clazz;

#define IZ_GET_REPLICATED_OBJ_CLASS(clazz) clazz::Class##clazz()

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_H__)
