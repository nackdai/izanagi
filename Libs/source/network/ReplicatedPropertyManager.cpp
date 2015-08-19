#include "network/ReplicatedPropertyManager.h"
#include "network/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty.h"

namespace izanagi {
namespace net {

    IMemoryAllocator* ReplicatedPropertyManager::s_Allocator = nullptr;
    std::atomic<IZ_UINT64> ReplicatedPropertyManager::s_ID(0);

    IZ_UINT64 ReplicatedPropertyManager::genID()
    {
        s_ID.store(s_ID + 1);
        return s_ID;
    }

    void ReplicatedPropertyManager::setAllocator(IMemoryAllocator* allocator)
    {
        s_Allocator = allocator;
    }

    void ReplicatedPropertyManager::add(ReplicatedObjectBase* obj)
    {
        m_hash.Add(obj->getReplicatedObjectHashItem());

        // TODO
        // ここで通信チャンネルを作る？
    }

    /////////////////////////////////////////////////////////

    class ReplicatedPropertyServer : public ReplicatedPropertyManager {
    public:
        ReplicatedPropertyServer();
        virtual ~ReplicatedPropertyServer();

        virtual void update() override;

        virtual IZ_BOOL isServer() override
        {
            return IZ_TRUE;
        }

        void send(ReplicatedObjectBase* obj);
    };

    void ReplicatedPropertyServer::update()
    {
        auto item = m_hash.GetOrderTop();

        while (item != IZ_NULL) {
            auto obj = item->GetData()->GetData();

            if (obj->isDirtyReplicatedProperty()) {
                send(obj);

                obj->undirtyReplicatedProperty();
            }

            item = item->GetNext();
        }
    }

    void ReplicatedPropertyServer::send(ReplicatedObjectBase* obj)
    {
        auto item = obj->getReplicatedPropertyListTopItem();

        while (item != IZ_NULL) {
            auto prop = item->GetData();

            if (prop->isDirty()) {
                // TODO
                // 通信

                prop->unDirty();
            }

            item = item->GetNext();
        }
    }

    /////////////////////////////////////////////////////////

    class ReplicatedPropertyClient : public ReplicatedPropertyManager {
    public:
        ReplicatedPropertyClient();
        virtual ~ReplicatedPropertyClient();

        virtual void update() override;

        virtual IZ_BOOL isServer() override
        {
            return IZ_FALSE;
        }

        void recv(ReplicatedObjectBase* obj);
    };

    void ReplicatedPropertyClient::update()
    {
        auto item = m_hash.GetOrderTop();

        while (item != IZ_NULL) {
            auto obj = item->GetData()->GetData();

            // TODO
            // クライアント側では値の変更を許さない？
            IZ_ASSERT(!obj->isDirtyReplicatedProperty());

            recv(obj);

            item = item->GetNext();
        }
    }

    void ReplicatedPropertyClient::recv(ReplicatedObjectBase* obj)
    {
        auto item = obj->getReplicatedPropertyListTopItem();

        while (item != IZ_NULL) {
            auto prop = item->GetData();

            // TODO

            item = item->GetNext();
        }
    }

    /////////////////////////////////////////////////////////

    ReplicatedPropertyServer* g_Server = IZ_NULL;
    ReplicatedPropertyClient* g_Client = IZ_NULL;

    ReplicatedPropertyManager* ReplicatedPropertyManager::create(IZ_BOOL isServer)
    {
        IZ_ASSERT(s_Allocator);
        IZ_ASSERT(g_Server == IZ_NULL && g_Client == IZ_NULL);

        if (isServer) {
            // TODO
            return g_Server;
        }
        else {
            // TODO
            return g_Client;
        }
    }

    void ReplicatedPropertyManager::finish()
    {
        IZ_ASSERT(s_Allocator);

        FREE(s_Allocator, g_Server);
        FREE(s_Allocator, g_Client);
    }

    ReplicatedPropertyManager* ReplicatedPropertyManager::get()
    {
        if (g_Server != IZ_NULL) {
            IZ_ASSERT(g_Client == IZ_NULL);
            return g_Server;
        }
        else if (g_Client != IZ_NULL) {
            IZ_ASSERT(g_Server == IZ_NULL);
            return g_Client;
        }

        IZ_ASSERT(IZ_FALSE);
        return IZ_NULL;
    }

    void ReplicatedPropertyManager::updateManager()
    {
        if (g_Server != IZ_NULL) {
            IZ_ASSERT(g_Client == IZ_NULL);
            g_Server->update();
        }
        else if (g_Client != IZ_NULL) {
            IZ_ASSERT(g_Server == IZ_NULL);
            g_Client->update();
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
    }
}    // namespace net
}    // namespace izanagi
