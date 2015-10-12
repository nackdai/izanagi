#include "network/ReplicatedProperty/ReplicatedPropertyManager.h"
#include "network/ReplicatedProperty/ReplicatedPropertyObject.h"
#include "network/ReplicatedProperty/ReplicatedProperty.h"

#include "network/NetworkUDP_libuv.h"
#include "network/rudp/ReliableUDPListener.h"

namespace izanagi {
namespace net {

    IMemoryAllocator* ReplicatedPropertyManager::s_Allocator = nullptr;
    std::atomic<IZ_UINT64> ReplicatedPropertyManager::s_ID{ 0 };

    IZ_UINT64 ReplicatedPropertyManager::genID()
    {
        IZ_ASSERT(isInitialized());

        s_ID.store(s_ID + 1);
        return s_ID;
    }

    IZ_BOOL ReplicatedPropertyManager::isInitialized()
    {
        return (s_Allocator != nullptr);
    }

    IZ_UINT ReplicatedPropertyManager::getObjectNum()
    {
        sys::Lock lock(m_locker);

        IZ_UINT ret = m_list.GetItemNum();
        return ret;
    }

    void ReplicatedPropertyManager::deleteObject(ReplicatedObjectBase* obj)
    {
        delete obj;
        FREE(s_Allocator, obj);
    }

    std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*> ReplicatedPropertyManager::pop()
    {
        auto ret = std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*>(ReplicatedObjectClass::Invalid, nullptr);
        return ret;
    }

    /////////////////////////////////////////////////////////

    enum ReplicatedPropertyProtocol : IZ_UINT {
        Create,
    };

    struct SReplicatedPropertyHeader {
        ReplicatedPropertyProtocol type;
    };

    struct SCreateObject {
        SReplicatedPropertyHeader header;
        IZ_UINT key;
    };

    /////////////////////////////////////////////////////////

    // ReplicatedPropertyManager on the server.
    class ReplicatedPropertyServer : public ReplicatedPropertyManager {
    public:
        ReplicatedPropertyServer() {}
        virtual ~ReplicatedPropertyServer() {}

        virtual IZ_BOOL init(const IPv4Endpoint& ep) override;

        // Update replicated property system.
        virtual void update() override;

        // Get if this is on server.
        virtual IZ_BOOL isServer() override
        {
            return IZ_TRUE;
        }

        // Send dirty replicated properties.
        void send(ReplicatedObjectBase* obj);

        virtual void registerCreator(const ReplicatedObjectClass& clazz, ObjectCreatorBase* creator) override;

        virtual ReplicatedObjectBase* create(const ReplicatedObjectClass& clazz) override;

    private:
        std::mutex m_hashCreatorLocker;
        CreatorHash m_hashCreator;

        Udp m_udp;
        ReliableUDPListener m_listener;
    };

    IZ_BOOL ReplicatedPropertyServer::init(const IPv4Endpoint& ep)
    {
        VRETURN(m_udp.start(s_Allocator, ep));

        m_listener.Init(s_Allocator, &m_udp);

        return IZ_TRUE;
    }

    // Update replicated property system.
    void ReplicatedPropertyServer::update()
    {
        sys::Lock lock(m_locker);

        auto item = m_list.GetTop();

        while (item != IZ_NULL) {
            auto obj = item->GetData();

            if (obj->hasDirtyReplicatedProperty()) {
                send(obj);

                // Send all dirty replicated property, so make object un-dirty.
                obj->undirtyReplicatedProperty();
            }

            item = item->GetNext();
        }
    }

    // Send dirty replicated properties.
    void ReplicatedPropertyServer::send(ReplicatedObjectBase* obj)
    {
        auto item = obj->getReplicatedPropertyListTopItem();

        while (item != IZ_NULL) {
            auto prop = item->GetData();

            if (prop->isDirty()) {
                // TODO
                // 通信

                // Send dirty property, so make it un-dirty.
                prop->unDirty();
            }

            item = item->GetNext();
        }
    }

    void ReplicatedPropertyServer::registerCreator(const ReplicatedObjectClass& clazz, ObjectCreatorBase* creator)
    {
        std::lock_guard<std::mutex> locker(m_hashCreatorLocker);

        auto item = m_hashCreator.Find(clazz);

        if (item == IZ_NULL) {
            creator->m_item.Init(clazz, creator);

            m_hashCreator.Add(&creator->m_item);
        }
    }

    ReplicatedObjectBase* ReplicatedPropertyServer::create(const ReplicatedObjectClass& clazz)
    {
        ReplicatedObjectBase* ret = nullptr;

        auto item = m_hashCreator.Find(clazz);

        if (item) {
            auto creator = item->GetData();

            ret = creator->create(s_Allocator);

            m_list.AddTail(ret->getListItem());

            SCreateObject protocol;
            protocol.key = ret->getClass();

            // 作成したことを通知.
            m_listener.sendToAll(
                &protocol,
                sizeof(protocol));
        }

        return ret;
    }

    /////////////////////////////////////////////////////////

    // ReplicatedPropertyManager on the client.
    class ReplicatedPropertyClient : public ReplicatedPropertyManager {
    public:
        ReplicatedPropertyClient();
        virtual ~ReplicatedPropertyClient();

        virtual IZ_BOOL init(const IPv4Endpoint& ep) override;

        // Update replicated property system.
        virtual void update() override;

        // Get if this is on the server.
        virtual IZ_BOOL isServer() override
        {
            return IZ_FALSE;
        }

        void recv(ReplicatedObjectBase* obj);

        virtual std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*> pop() override;

    private:
        void OnProc();

    private:
        std::thread m_recvThread;
        IZ_BOOL m_isRunning{ IZ_FALSE };

        std::mutex m_objectListLocker;
        CStdList<ReplicatedObjectBase> m_objectList;

        Udp m_udp;
        ReliableUDP m_rudp;
    };

    ReplicatedPropertyClient::ReplicatedPropertyClient()
    {
    }

    ReplicatedPropertyClient::~ReplicatedPropertyClient()
    {
    }

    IZ_BOOL ReplicatedPropertyClient::init(const IPv4Endpoint& ep)
    {
        VRETURN(m_udp.start(s_Allocator));

        m_rudp.Init(
            s_Allocator,
            &m_udp,
            RUDPParameter());
        m_rudp.Connect(ep);

        // TODO
        // RUDPがコネクトしたタイミングでスレッドを起こしたい.
        m_isRunning = IZ_TRUE;
        m_recvThread = std::thread(std::bind(&ReplicatedPropertyClient::OnProc, this));

        return IZ_TRUE;
    }

    // Update replicated property system.
    void ReplicatedPropertyClient::update()
    {
        sys::Lock lock(m_locker);

        auto item = m_list.GetTop();

        while (item != IZ_NULL) {
            auto obj = item->GetData();

            // TODO
            // クライアント側では値の変更を許さない？
            IZ_ASSERT(!obj->hasDirtyReplicatedProperty());

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

    std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*> ReplicatedPropertyClient::pop()
    {
        std::lock_guard<std::mutex> lock(m_objectListLocker);

        auto item = m_objectList.GetTop();

        if (item == IZ_NULL) {
            return ReplicatedPropertyManager::pop();
        }

        auto obj = item->GetData();
        item->Leave();

        auto ret = std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*>(
            obj->getClass(),
            obj);

        return ret;
    }

    void ReplicatedPropertyClient::OnProc()
    {
        IZ_UINT8 bytes[1024];

        while (m_isRunning) {
            auto size = m_rudp.Recieve(bytes, sizeof(bytes));

            if (size > 0) {
                SReplicatedPropertyHeader* header = (SReplicatedPropertyHeader*)bytes;

                switch (header->type)
                {
                case ReplicatedPropertyProtocol::Create:
                {
                    SCreateObject* protocol = (SCreateObject*)header;

                    // TODO
                    // オブジェクト作成
                }
                    break;
                default:
                    break;
                }
            }
        }
    }

    /////////////////////////////////////////////////////////

    ReplicatedPropertyServer* g_Server = nullptr;
    ReplicatedPropertyClient* g_Client = nullptr;

    ReplicatedPropertyManager* ReplicatedPropertyManager::begin(
        IZ_BOOL isServer,
        IMemoryAllocator* allocator)
    {
        s_Allocator = allocator;

        IZ_ASSERT(g_Server == nullptr && g_Client == nullptr);

        if (isServer) {
            // TODO
            return g_Server;
        }
        else {
            // TODO
            return g_Client;
        }
    }

    void ReplicatedPropertyManager::end()
    {
        IZ_ASSERT(s_Allocator);

        FREE(s_Allocator, g_Server);
        FREE(s_Allocator, g_Client);
    }

    ReplicatedPropertyManager* ReplicatedPropertyManager::get()
    {
        if (g_Server) {
            IZ_ASSERT(g_Client == nullptr);
            return g_Server;
        }
        else if (g_Client) {
            IZ_ASSERT(g_Server == nullptr);
            return g_Client;
        }

        IZ_ASSERT(IZ_FALSE);
        return IZ_NULL;
    }
}    // namespace net
}    // namespace izanagi
