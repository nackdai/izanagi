#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

static const IZ_CHAR* serverIp = "127.0.0.1";

static const IZ_UINT ServerPort = 30000;

class TestObject : public izanagi::net::ReplicatedObject < > {
    IZ_DEFS_REPLICATED_OBJ(TestObject);

public:
    TestObject() {}
    virtual ~TestObject() {}

    IZ_REPLICATED_PROPERTY(TestObject, IZ_UINT32, v0, izanagi::net::E_REPLICATED_TYPE::Rep, izanagi::net::E_REPLICATED_RELIABLE::Reliable);
};

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint host(
        izanagi::net::IPv4Address::Any,
        ServerPort);

    auto manager = izanagi::net::ReplicatedPropertyManager::getListener();

    manager->init(host);

    manager->accept(1, -1.0f);

    TestObject* obj = IZ_CREATE_REPLIACTED_OBJECT(manager, TestObject);

    obj->v0 = 100;

    manager->update();

    for (;;) {}
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint remote(
        izanagi::net::IPv4Address(serverIp),
        ServerPort);

    auto manager = izanagi::net::ReplicatedPropertyManager::get();

    manager->init(remote);

    izanagi::net::ReplicatedPropertyManager::CreatedReplicatedObjectHandler handler;

    do {
        handler = manager->pop();
    } while (std::get<1>(handler) == nullptr);

    auto obj = (TestObject*)std::get<1>(handler);

    for (;;) {
        if (obj->v0 == 100) {
            IZ_PRINTF("Sync!!\n");
            break;
        }
    }
}

IzMain(0, 0)
{
    IZ_BOOL isServer = IZ_FALSE;

	if (argc > 1 && strcmp(argv[1], "s" ) == 0) {
		isServer = IZ_TRUE;
	}

	izanagi::CStandardMemoryAllocator allocator;
	allocator.Init(sizeof(buffer), buffer);

	izanagi::net::Network::begin();

    izanagi::net::ReplicatedPropertyManager::begin(
        isServer,
        &allocator);

    IZ_REGIST_REPLICATED_OBJ(TestObject);

	if (isServer) {
		runAsServer(&allocator);
	}
	else {
		runAsClient(&allocator);
	}

    izanagi::net::ReplicatedPropertyManager::end();

	izanagi::net::Network::end();

	allocator.Dump();

	return 0;
}
