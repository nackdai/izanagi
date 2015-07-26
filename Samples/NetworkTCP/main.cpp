#include "izSampleKit.h"
#include "izNetwork.h"

IZ_UINT8 buffer[2 * 1024 * 1024];

IzMain(0, 0)
{
	izanagi::CStandardMemoryAllocator allocator;
	allocator.Init(sizeof(buffer), buffer);

	izanagi::net::Network::begin();

	izanagi::net::IPv4Address addr;
	izanagi::net::IPv4Endpoint ep(addr, 30000);

	izanagi::net::Tcp tcp;
	tcp.start(&allocator, ep, 1);

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

	izanagi::net::Network::end();

	allocator.Dump();

	th.join();

	return 0;
}
