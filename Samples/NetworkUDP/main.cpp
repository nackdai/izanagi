#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

static const IZ_UINT Port = 30000;

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Endpoint ep(Port);

	izanagi::net::Udp udp;
    udp.startAsServer(allocator, ep);

	IZ_BOOL willQuit = IZ_FALSE;

	std::thread th([&] {
		if (::getchar() == 'q') {
			willQuit = IZ_TRUE;
		}
	});

    IZ_CHAR buf[1024];

	for (;;) {
		if (willQuit) {
			break;
		}

        udp.run(buf, sizeof(buf));

        izanagi::sys::CThread::Sleep(33);
	}

    udp.stop();

	th.join();
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
    izanagi::net::IPv4Address addr;
    addr.setByHostName("localhost");

    izanagi::net::IPv4Endpoint ep(addr, Port);

    izanagi::net::Udp udp;
    udp.startAsClient(allocator, ep);

    static const IZ_CHAR* str = "test";

    // TODO
    // ‘—M‚³‚ê‚é‚Ü‚Å‘Ò‚Â....
    izanagi::sys::CThread::Sleep(1000);

    udp.stop();
}

IzMain(0, 0)
{
    IZ_BOOL isServer = IZ_FALSE;

	if (argc > 0 && strcmp(argv[1], "s" ) == 0) {
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
