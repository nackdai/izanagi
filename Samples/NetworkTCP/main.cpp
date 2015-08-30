#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

static const IZ_CHAR* serverIp = "127.0.0.1";

static const IZ_UINT ServerPort = 30000;
static const IZ_UINT ClientPort = 31000;

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint host(
        izanagi::net::IPv4Address::Any,
        ServerPort);

	izanagi::net::TcpListener listener;
    listener.start(allocator, host, 1);

    auto client = listener.acceptRemote();

    static const IZ_UINT size = 1024;
    IZ_UINT8 buf[size] = { 0 };

    auto len = client->recieve(buf, size);

    if (len > 0) {
        IZ_PRINTF("%s\n", buf);
    }

    listener.stop();
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint host(
        izanagi::net::IPv4Address("localhost"),
        ClientPort);

    izanagi::net::IPv4Endpoint remote(
        izanagi::net::IPv4Address(serverIp),
        ServerPort);

	izanagi::net::TcpClient client;
    client.start(host);
    client.connectTo(remote);

	static const IZ_CHAR* str = "test";

    client.sendData(str, strlen(str));

    client.stop();
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

	if (isServer) {
		runAsServer(&allocator);
	}
	else {
		runAsClient(&allocator);
	}

	izanagi::net::Network::end();

	allocator.Dump();

	return 0;
}
