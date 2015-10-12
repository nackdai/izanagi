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

IZ_DECL_REPLICATED_OBJ(TestObject);

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
    auto manager = izanagi::net::ReplicatedPropertyManager::get();

    TestObject* obj = IZ_CREATE_REPLIACTED_OBJECT(manager, TestObject);

    obj->v0 = 10;
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    auto manager = izanagi::net::ReplicatedPropertyManager::get();

    auto r = manager->pop();

    auto obj = std::get<1>(r);


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
