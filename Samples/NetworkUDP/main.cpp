#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

static const IZ_CHAR* serverIp = "127.0.0.1";

static const IZ_UINT ServerPort = 30000;

static IZ_BOOL isAsync = IZ_FALSE;

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint host(
        izanagi::net::IPv4Address::Any,
        ServerPort);

	izanagi::net::Udp udp;
    udp.start(host);

    static const IZ_UINT size = 1024;
    IZ_CHAR buf[size] = { 0 };

    izanagi::net::IPv4Endpoint remote;

    if (isAsync) {
        IZ_BOOL running = IZ_TRUE;

        udp.beginRecieve(
            [&](const izanagi::net::Packet* packet){
                if (packet) {
                    IZ_CHAR ip[size];
                    packet->endpoint.getAddress().toString(ip, size);

                    auto port = packet->endpoint.getPort();

                    IZ_PRINTF("%s:%d (%s)\n", ip, port, packet->data);
                }
                running = IZ_FALSE;
        }, buf, size);

        while (running) {
            izanagi::sys::CThread::Sleep(33);
        }

        udp.endRecieve();
    }
    else {
        auto len = udp.recieveFrom(buf, size, remote);

        if (len > 0) {
            IZ_CHAR ip[size];
            remote.getAddress().toString(ip, size);

            auto port = remote.getPort();

            IZ_PRINTF("%s:%d (%s)\n", ip, port, buf);
        }
    }

    udp.stop();
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint remote(
        izanagi::net::IPv4Address(serverIp),
        ServerPort);

    izanagi::net::Udp udp;
    udp.start();
    udp.connectTo(remote);

    static const IZ_CHAR* str = "test";

    udp.sendData(str, strlen(str));

    udp.stop();
}

IzMain(0, 0)
{
    IZ_BOOL isServer = IZ_FALSE;

	if (argc > 1 && strcmp(argv[1], "s" ) == 0) {
		isServer = IZ_TRUE;
	}

    if (argc > 2 && strcmp(argv[2], "a") == 0) {
        isAsync = IZ_TRUE;
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
