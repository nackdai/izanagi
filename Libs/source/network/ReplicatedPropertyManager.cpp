#include "network/ReplicatedPropertyManager.h"

namespace izanagi {
namespace net {
    IZ_UINT64 ReplicatedPropertyManager::s_ID = 0;
    ReplicatedPropertyManager ReplicatedPropertyManager::s_instance;

    ReplicatedPropertyManager& ReplicatedPropertyManager::getInstance()
    {
        return s_instance;
    }

    ReplicatedPropertyManager::ReplicatedPropertyManager()
    {
        m_allocator = nullptr;
        m_isRunning = IZ_FALSE;
        m_isServer = IZ_FALSE;
    }

    ReplicatedPropertyManager::~ReplicatedPropertyManager()
    {
        // TODO
    }

    IZ_BOOL ReplicatedPropertyManager::start(
        IZ_BOOL isServer,
        IMemoryAllocator* allocator)
    {
        if (m_isRunning.load()) {
            return IZ_FALSE;
        }

        m_allocator = allocator;
        m_isServer = isServer;

        m_isRunning.store(IZ_TRUE);

        return IZ_TRUE;
    }

    IZ_UINT64 ReplicatedPropertyManager::genID()
    {
        s_ID++;
        return s_ID;
    }

    template <typename _T>
    void ReplicatedPropertyManager::init(ReplicatedProperty<_T>& prop)
    {
        prop.m_id = genID();

        SProperty* p = (SProperty*)ALLOC(m_allocator, sizeof(SProperty));
        IZ_ASSERT(p != IZ_NULL);

        p->id = prop.m_id;
        p->ptr = &prop;
        p->hashItem.Init(prop.m_id, p);
        p->listItem.Init(p);

        m_hash.Add(&p->hashItem);
    }

    void ReplicatedPropertyManager::send(IZ_UINT64 id, IZ_BOOL isReliable)
    {
        auto prop = m_hash.FindData(id);
        IZ_ASSERT(prop != IZ_NULL);

        // TODO
    }

    IZ_BOOL ReplicatedPropertyManager::isServer()
    {
        return m_isServer;
    }
}    // namespace net
}    // namespace izanagi
