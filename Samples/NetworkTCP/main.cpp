#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

void runAsServer(izanagi::IMemoryAllocator* allocator)
{
	izanagi::net::IPv4Address addr;
	addr.setHost("localhost");

	izanagi::net::IPv4Endpoint ep(addr, 30000);

	izanagi::net::Tcp tcp;
	tcp.startAsServer(allocator, ep, 1);

	IZ_BOOL willQuit = IZ_FALSE;

	std::thread th([&] {
		if (::getchar() == 'q') {
			willQuit = IZ_TRUE;
		}
	});

	for (;;) {
		if (willQuit) {
			break;
		}

		tcp.recieve([](const izanagi::net::Packet& packet) {
			IZ_PRINTF("%s\n", packet.data);
		});
	}

	tcp.stop();

	th.join();
}

void runAsClient(izanagi::IMemoryAllocator* allocator)
{
	izanagi::net::IPv4Address addr;
	addr.setHost("localhost");

	izanagi::net::IPv4Endpoint ep(addr, 30000);

	izanagi::net::Tcp tcp;
	tcp.startAsClient(allocator, ep, IZ_FALSE);

	static const IZ_CHAR* str = "test";

	tcp.sendData(str, strlen(str));

	// TODO
	// ‘—M‚³‚ê‚é‚Ü‚Å‘Ò‚Â....
	izanagi::sys::CThread::Sleep(1000);

	tcp.stop();
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
