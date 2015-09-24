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

    izanagi::net::TcpClient* client = nullptr;

    while (client == nullptr) {
        client = listener.acceptRemote();
    }

    IZ_UINT8 buf[28] = { 0 };
    
    while (client->recieve(buf, sizeof(buf)) < 0) {

    }

    IZ_PRINTF("%s\n", buf);

    client->stop();
       
    listener.stop();
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint remote(
        izanagi::net::IPv4Address(serverIp),
        ServerPort);

	izanagi::net::TcpClient client;
    client.start(allocator);

    IZ_BOOL isConnected = IZ_FALSE;

    client.connectTo(
        remote,
        [&](IZ_BOOL result) {
        isConnected = IZ_TRUE;
    });

    while (!isConnected) {
    }

	static const IZ_CHAR* str = "test";

    IZ_BOOL isSent = IZ_FALSE;

    client.sendData(
        str, strlen(str),
        [&](IZ_BOOL result) {
        IZ_PRINTF("%s\n", result ? "Send success" : "Send fail");
        isSent = IZ_TRUE;
    });

    while (!isSent) {
    }

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
#endif