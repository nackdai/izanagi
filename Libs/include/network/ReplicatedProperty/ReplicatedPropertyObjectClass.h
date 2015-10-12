#if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_CLASS_H__)
#define _IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_CLASS_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
namespace net {
    /**
     */
    class ReplicatedObjectClass {
    public:
        static ReplicatedObjectClass Invalid;

        static IZ_BOOL isValid(const ReplicatedObjectClass& clazz);

    public:
        ReplicatedObjectClass() : m_name(nullptr) {}
        ReplicatedObjectClass(const IZ_CHAR* tag);
        ReplicatedObjectClass(IZ_UINT key);
        ~ReplicatedObjectClass() {}

        ReplicatedObjectClass(const ReplicatedObjectClass& rhs);
        const ReplicatedObjectClass& operator=(const ReplicatedObjectClass& rhs);

    public:
        IZ_BOOL operator==(const ReplicatedObjectClass& rhs) const;

        IZ_BOOL operator!=(const ReplicatedObjectClass& rhs) const;

        IZ_BOOL is(const ReplicatedObjectClass& clazz);

        operator IZ_UINT() const;

    private:
        CKey m_key;
        IZ_CHAR* m_name;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_REPLICATED_PROPERTY_OBJECT_CLASS_H__)
