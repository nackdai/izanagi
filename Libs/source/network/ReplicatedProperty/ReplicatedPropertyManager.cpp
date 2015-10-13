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

        s_ID++;
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

    ReplicatedObjectBase* ReplicatedPropertyManager::create(const ReplicatedObjectClass& clazz)
    {
        ReplicatedObjectBase* ret = nullptr;

        auto item = m_hashCreator.Find(clazz);

        if (item) {
            auto creator = item->GetData();

            ret = creator->create(s_Allocator);

            {
                sys::Lock lock(m_locker);
                m_list.AddTail(ret->getListItem());
            }

            OnCreate(ret);
        }

        return ret;
    }

    void ReplicatedPropertyManager::deleteObject(ReplicatedObjectBase* obj)
    {
        delete obj;
        FREE(s_Allocator, obj);
    }

    ReplicatedPropertyManager::CreatedReplicatedObjectHandler ReplicatedPropertyManager::pop()
    {
        auto ret = std::tuple<const ReplicatedObjectClass&, ReplicatedObjectBase*>(ReplicatedObjectClass::Invalid, nullptr);
        return ret;
    }

    void ReplicatedPropertyManager::remove(ReplicatedObjectBase& obj)
    {
        sys::Lock lock(m_locker);
        m_list.Remove(obj.getListItem());
    }

    void ReplicatedPropertyManager::registerCreator(const ReplicatedObjectClass& clazz, ObjectCreatorBase* creator)
    {
        std::lock_guard<std::mutex> locker(m_hashCreatorLocker);

        auto item = m_hashCreator.Find(clazz);

        if (item == IZ_NULL) {
            creator->m_item.Init(clazz, creator);

            m_hashCreator.Add(&creator->m_item);
        }
    }

    /////////////////////////////////////////////////////////

    enum ReplicatedPropertyProtocol : IZ_UINT {
        Create,
        Sync,
    };

    struct SReplicatedPropertyHeader {
        ReplicatedPropertyProtocol type;
    };

    struct SCreateObject {
        SReplicatedPropertyHeader header;
        IZ_UINT key;
        IZ_UINT64 id;

        SCreateObject()
        {
            header.type = ReplicatedPropertyProtocol::Create;
            key = 0;
            id = 0;
        }
    };

    struct SSyncProperty {
        SReplicatedPropertyHeader header;
        IZ_UINT num;
        IZ_UINT64 id;

        SSyncProperty()
        {
            header.type = ReplicatedPropertyProtocol::Sync;
            num = 0;
            id = 0;
        }
    };

    struct SReplicatedProp {
        IZ_UINT16 idx;
        IZ_UINT16 length;

        // TODO
        IZ_UINT8 buf[16];
    };

    /////////////////////////////////////////////////////////

    // ReplicatedPropertyManager on the server.
    class ReplicatedPropertyServer : public ReplicatedPropertyManagerListener {
    public:
        ReplicatedPropertyServer() {}
        virtual ~ReplicatedPropertyServer() {}

        virtual IZ_BOOL init(const IPv4Endpoint& ep) override;

        virtual void accept(IZ_UINT acceptedNum, IZ_FLOAT timeout) override;

        // Update replicated property system.
        virtual void update() override;

        // Get if this is on server.
        virtual IZ_BOOL isServer() override
        {
            return IZ_TRUE;
        }

        // Send dirty replicated properties.
        void send(ReplicatedObjectBase* obj);

        void send(
            const SSyncProperty& protocol,
            const SReplicatedProp* props);

        virtual void OnCreate(ReplicatedObjectBase* cretatedObj) override;

        IZ_DECL_PLACEMENT_NEW();

    private:
        void OnProc();

    private:
        Udp m_udp;
        ReliableUDPListener m_listener;
    };

    IZ_BOOL ReplicatedPropertyServer::init(const IPv4Endpoint& ep)
    {
        VRETURN(m_udp.start(s_Allocator, ep));

        m_listener.Init(s_Allocator, &m_udp);

        return IZ_TRUE;
    }

    void ReplicatedPropertyServer::accept(IZ_UINT acceptedNum, IZ_FLOAT timeout)
    {
        sys::CTimer timer;

        for (;;) {
            timer.Begin();

            auto client = m_listener.Accept();

            if (m_listener.acceptedNum() >= acceptedNum) {
                break;
            }

            auto time = timer.End();

            if (timeout >= 0.0f && time >= timeout) {
                break;
            }
        }
    }

    // Update replicated property system.
    void ReplicatedPropertyServer::update()
    {
        sys::Lock lock(m_locker);
        
        auto item = m_list.GetTop();

        while (item) {
            auto obj = item->GetData();

            if (obj->hasDirtyReplicatedProperty()) {
                send(obj);
                obj->undirtyReplicatedProperty();
            }

            item = item->GetNext();
        }
    }

    // Send dirty replicated properties.
    void ReplicatedPropertyServer::send(ReplicatedObjectBase* obj)
    {
        SSyncProperty protocol;
        protocol.id = obj->getReplicatedObjectID();

        SReplicatedProp props[10];

        IZ_UINT cnt = 0;

        auto item = obj->getReplicatedPropertyListTopItem();

        while (item != IZ_NULL) {
            auto prop = item->GetData();

            if (prop->isDirty()) {
                if (protocol.num >= COUNTOF(props)) {
                    send(protocol, props);

                    protocol.num = 0;
                }

                SReplicatedProp& sendProp = props[protocol.num];
                protocol.num++;

                sendProp.idx = cnt;
                sendProp.length = prop->getTypeSize();

                IZ_ASSERT(sendProp.length <= sizeof(sendProp.buf));

                memcpy(sendProp.buf, prop->getValue(), sendProp.length);

                // Send dirty property, so make it un-dirty.
                prop->unDirty();
            }

            item = item->GetNext();
            cnt++;
        }

        if (protocol.num > 0) {
            send(protocol, props);
        }
    }

    void ReplicatedPropertyServer::send(
        const SSyncProperty& protocol,
        const SReplicatedProp* props)
    {
        // TODO
        IZ_UINT8 buf[1024];

        IZ_ASSERT(sizeof(buf) >= sizeof(protocol)+sizeof(SReplicatedProp)* protocol.num);

        IZ_UINT size = 0;

        memcpy(buf, &protocol, sizeof(protocol));
        size += sizeof(protocol);

        memcpy(buf + size, props, sizeof(SReplicatedProp)* protocol.num);
        size += sizeof(SReplicatedProp)* protocol.num;

        m_listener.sendToAll(buf, size);
    }

    void ReplicatedPropertyServer::OnCreate(ReplicatedObjectBase* cretatedObj)
    {
        SCreateObject protocol;
        protocol.key = cretatedObj->getClass();
        protocol.id = cretatedObj->getReplicatedObjectID();

        // 作成したことを通知.
        m_listener.sendToAll(
            &protocol,
            sizeof(protocol));
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

        void recv(
            const SSyncProperty& protocol,
            const SReplicatedProp* props);

        virtual CreatedReplicatedObjectHandler pop() override;

        IZ_DECL_PLACEMENT_NEW();

    private:
        void OnProc();

        virtual void OnCreate(ReplicatedObjectBase* cretatedObj) override;

    private:
        std::thread m_recvThread;
        IZ_BOOL m_isRunning{ IZ_FALSE };

        Udp m_udp;
        ReliableUDP m_rudp;

        std::mutex m_managedListLocker;
        CStdList<ReplicatedObjectBase> m_managedList;
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

            item = item->GetNext();
        }
    }

    void ReplicatedPropertyClient::recv(
        const SSyncProperty& protocol,
        const SReplicatedProp* props)
    {
        // TODO

        std::lock_guard<std::mutex> lock(m_managedListLocker);

        auto item = m_managedList.GetTop();

        while (item) {
            auto obj = item->GetData();

            if (obj->getReplicatedObjectID() == protocol.id) {
                auto propItem = obj->getReplicatedPropertyListTopItem();

                while (propItem) {
                    auto prop = propItem->GetData();

                    for (IZ_UINT i = 0; i < protocol.num; i++) {
                        auto& p = props[i];

                        if (p.idx == prop->getID()) {
                            prop->sync(p.buf, p.length);

                            break;
                        }
                    }

                    propItem = propItem->GetNext();
                }
            }

            item = item->GetNext();
        }
    }

    ReplicatedPropertyManager::CreatedReplicatedObjectHandler ReplicatedPropertyClient::pop()
    {
        CStdList<ReplicatedObjectBase>::Item* item = IZ_NULL;

        {
            sys::Lock lock(m_locker);

            item = m_list.GetTop();
        }

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
        IZ_UINT8 bytes[1024] = { 0 };

        while (m_isRunning) {
            auto size = m_rudp.Recieve(bytes, sizeof(bytes));

            if (size > 0) {
                SReplicatedPropertyHeader* header = (SReplicatedPropertyHeader*)bytes;

                switch (header->type)
                {
                case ReplicatedPropertyProtocol::Create:
                {
                    SCreateObject* protocol = (SCreateObject*)header;

                    // オブジェクト作成
                    auto obj = create(ReplicatedObjectClass(protocol->key));
                    obj->setReplicatedObjectID(protocol->id);
                }
                    break;
                case ReplicatedPropertyProtocol::Sync:
                {
                    SSyncProperty* protocol = (SSyncProperty*)header;
                    SReplicatedProp* props = (SReplicatedProp*)(bytes + sizeof(SSyncProperty));

                    recv(*protocol, props);
                }
                    break;
                default:
                    break;
                }
            }
        }
    }

    void ReplicatedPropertyClient::OnCreate(ReplicatedObjectBase* cretatedObj)
    {
        std::lock_guard<std::mutex> lock(m_managedListLocker);
        m_managedList.AddTail(cretatedObj->getListItemEx());
    }

    /////////////////////////////////////////////////////////

    ReplicatedPropertyServer* g_Server = nullptr;
    ReplicatedPropertyClient* g_Client = nullptr;

    ReplicatedPropertyManager* ReplicatedPropertyManager::begin(
        IZ_BOOL isServer,
        IMemoryAllocator* allocator)
    {
        VRETURN_NULL(s_Allocator == nullptr);

        s_Allocator = allocator;

        IZ_ASSERT(g_Server == nullptr && g_Client == nullptr);

        if (isServer) {
            void* p = ALLOC(s_Allocator, sizeof(ReplicatedPropertyServer));
            g_Server = new(p)ReplicatedPropertyServer;
            return g_Server;
        }
        else {
            void* p = ALLOC(s_Allocator, sizeof(ReplicatedPropertyClient));
            g_Client = new(p)ReplicatedPropertyClient;
            return g_Client;
        }
    }

    void ReplicatedPropertyManager::end()
    {
        IZ_ASSERT(s_Allocator);

        if (g_Server) {
            delete g_Server;
            FREE(s_Allocator, g_Server);
        }

        if (g_Client) {
            delete g_Client;
            FREE(s_Allocator, g_Client);
        }
    }

    ReplicatedPropertyManager* ReplicatedPropertyManager::get()
    {
        if (g_Server) {
            IZ_ASSERT(g_Server && g_Client == nullptr);
            return g_Server;
        }
        else if (g_Client) {
            IZ_ASSERT(g_Client && g_Server == nullptr);
            return g_Client;
        }

        IZ_ASSERT(IZ_FALSE);
        return nullptr;
    }

    ReplicatedPropertyManagerListener* ReplicatedPropertyManager::getListener()
    {
        if (g_Server) {
            IZ_ASSERT(g_Server && g_Client == nullptr);
            return g_Server;
        }

        IZ_ASSERT(IZ_FALSE);
        return nullptr;
    }
}    // namespace net
}    // namespace izanagi
