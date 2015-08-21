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

    IZ_UINT ReplicatedPropertyManager::getObjectNum()
    {
        IZ_UINT ret = m_hash.GetDataNum();
        return ret;
    }

    ReplicatedObjectBase* ReplicatedPropertyManager::getObject(IZ_UINT idx)
    {
        IZ_ASSERT(idx < getObjectNum());

        auto item = m_hash.GetOrderAtHashItem(idx);
        auto ret = item->GetData();

        return ret;
    }

    ReplicatedObjectBase* ReplicatedPropertyManager::popObject()
    {
        auto item = m_hash.GetOrderTopHashItem();

        if (item != IZ_NULL) {
            auto data = item->GetData();

            item->Leave();

            return data;
        }

        return nullptr;
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

        virtual void add(ReplicatedObjectBase* obj) override;
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

    void ReplicatedPropertyServer::add(ReplicatedObjectBase* obj)
    {
        IZ_BOOL isFound = (m_hash.Find(obj->getReplicatedObjectID()) != IZ_NULL);

        if (!isFound) {
            // TODO
            // ここで通信チャンネルを作る？

            // TODO
            // オブジェクトが作られたことを通知
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

        virtual IZ_BOOL registerCreator(const CClass& clazz, ReplicatedObjectCreator func) override;

    private:
        struct CreatorHolder;
        using CreatorHash = CStdHash < CKey, CreatorHolder, 4 >;
        using CreatorHashItem = CreatorHash::Item;

        struct CreatorHolder {
            ReplicatedObjectCreator func;
            CClass clazz;
            CreatorHashItem hashItem;
        };

        CreatorHash m_hashCreator;
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

    IZ_BOOL ReplicatedPropertyClient::registerCreator(const CClass& clazz, ReplicatedObjectCreator func)
    {
        auto item = m_hashCreator.Find(clazz);
        IZ_BOOL ret = (item == IZ_NULL);

        if (ret) {
            CreatorHolder* p = (CreatorHolder*)ALLOC(
                ReplicatedPropertyManager::s_Allocator, 
                sizeof(CreatorHolder));

            IZ_ASSERT(p != IZ_NULL);

            p->hashItem.Init(clazz, p);
            p->clazz = clazz;
            p->func = func;

            m_hashCreator.Add(&p->hashItem);
        }

        return ret;
    }

    /////////////////////////////////////////////////////////

    ReplicatedPropertyServer* g_Server = IZ_NULL;
    ReplicatedPropertyClient* g_Client = IZ_NULL;

    void ReplicatedPropertyManager::begin(
        IZ_BOOL isServer,
        IMemoryAllocator* allocator)
    {
        s_Allocator = allocator;

        IZ_ASSERT(g_Server == IZ_NULL && g_Client == IZ_NULL);

        if (isServer) {
            // TODO
            
        }
        else {
            // TODO
            
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
}    // namespace net
}    // namespace izanagi
