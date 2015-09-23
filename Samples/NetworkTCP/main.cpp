#include "izSampleKit.h"
#include "izNetwork.h"

#if 1

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

// uv_handle_t > uv_stream_t > uv_tcp_t

const int backlog = 128;
const int buffer_size = 1024;
uv_fs_t open_req;
uv_fs_t read_req;
uv_tcp_t *client; // �ڑ����Ă����N���C�A���g(���񂵂��y�ɂ��邽��

void on_new_connection(uv_stream_t *server, int status);
uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size);
void on_client_read(uv_stream_t *client, ssize_t nread, uv_buf_t buf);
void on_client_write(uv_write_t *req, int status);
void on_file_open(uv_fs_t *req);
void on_file_read(uv_fs_t *req);

// �N���C�A���g�Ɛڑ���, �N���C�A���g����̃f�[�^��ǂݍ���
void on_new_connection(uv_stream_t *server, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_new_connection");
        //uv_close((uv_handle_t*)client, NULL);
        return;
    }
#if 0

    // �N���C�A���g��ێ����邽�߂̃��������m��
    client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    // loop �ւ̓o�^
    uv_tcp_init(uv_default_loop(), client);

    // accept
    int result = uv_accept(server, (uv_stream_t*)client);

    if (result == 0) { // success
        // �N���C�A���g����̃f�[�^��ǂ݂����āAalloc_buffer �Ŋm�ۂ����̈�ɏ������݁Acallback ���Ă�
        //uv_read_start((uv_stream_t*)client, alloc_buffer, on_client_read);
    }
    else { // error
        uv_close((uv_handle_t*)client, NULL);
    }
#endif
}

#if 0
// suggeseted_size �œn���ꂽ�̈���m��
uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size) {
    return uv_buf_init((char*)malloc(suggested_size), suggested_size);
}

// tcp �N���C�A���g����󂯎�����t�@�C�������J��
void on_client_read(uv_stream_t *_client, ssize_t nread, uv_buf_t buf) {
    if (nread == -1) {
        fprintf(stderr, "error on_client_read");
        uv_close((uv_handle_t*)client, NULL);
        return;
    }

    // �N���C�A���g����󂯎�����f�[�^
    // �f�[�^�� buf.base �Ɋi�[����Ă���B
    char *filename = buf.base;
    int mode = 0;

    // �t�@�C�����J��
    //uv_fs_open(uv_default_loop(), &open_req, filename, O_RDONLY, mode, on_file_open);
}

// client �ɕԂ������Ƃ̌�Еt��
void on_client_write(uv_write_t *req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_client_write");
        uv_close((uv_handle_t*)client, NULL);
        return;
    }

    // �J��
    free(req);
    // �����ŊJ�����邽�߂ɂ��Ă������B
    char *buffer = (char*)req->data;
    free(buffer);

    // tcp connection �����
    uv_close((uv_handle_t*)client, NULL);
}

// �J�����t�@�C���̓��e��ǂ�
void on_file_open(uv_fs_t *req) {
    if (req->result == -1) {
        fprintf(stderr, "error on_file_read");
        uv_close((uv_handle_t*)client, NULL);
        return;
    }

    // �t�@�C������ǂݍ��񂾃f�[�^������o�b�t�@
    char *buffer = (char *)malloc(sizeof(char) * buffer_size);

    // �ǂݍ��݂�o�^,�������݂̃o�b�t�@���w��
    int offset = -1;
    // data �t�B�[���h�ɓ���ăR�[���o�b�N�֐��ɓn��
    read_req.data = (void*)buffer;
    // �����͔񓯊��R�[���o�b�N
    //uv_fs_read(uv_default_loop(), &read_req, req->result, buffer, sizeof(char) * buffer_size, offset, on_file_read);
    // read ��o�^������J��
    uv_fs_req_cleanup(req);
}

// �t�@�C���̓��e���N���C�A���g�ɕԂ�
void on_file_read(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "error on_file_read");
        uv_close((uv_handle_t*)client, NULL);
    }
    else if (req->result == 0) { // �ǂݏI����������
        uv_fs_t close_req;
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
        uv_close((uv_handle_t*)client, NULL);
    }
    else { // �ǂݍ��񂾓��e���N���C�A���g�ɕԂ�
        // �������ݗp�̗̈�m��
        uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));

        // �N���C�A���g�ɕԂ����e
        // req->data �ɓ����Ă���o�b�t�@�̃|�C���^���ǂݍ���
        char *message = (char*)req->data;

        // uv_write �Ɏg�� uv_buf_t �̏���
        uv_buf_t buf = uv_buf_init(message, sizeof(message));
        buf.len = req->result;
        buf.base = message;
        int buf_count = 1;

        // on_client_write ���ŊJ���ł���悤�ɁA
        // �|�C���^���i�[���ēn���B
        write_req->data = (void*)message;

        // client �� buf ����������
        uv_write(write_req, (uv_stream_t*)client, &buf, buf_count, on_client_write);
    }
    // �����ŊJ��
    uv_fs_req_cleanup(req);
}
#endif
int main(void) {
    uv_loop_init(uv_default_loop());

    // Network I/O �̍\����
    uv_tcp_t server;
    // loop �ւ̓o�^
    uv_tcp_init(uv_default_loop(), &server);

    // �A�h���X�̎擾)
    sockaddr_in bind_addr;
    uv_ip4_addr("0.0.0.0", 30000, &bind_addr);

    // bind
    uv_tcp_bind(&server, (const sockaddr*)&bind_addr, 0);

    // uv_stream_t: uv_handle_t �̃T�u�N���X uv_tcp_t �̐e
    // listen
    int r = uv_listen((uv_stream_t*)&server, backlog,
        [](uv_stream_t *server, int status) {
        on_new_connection(server, status);
    });
    if (r) {
        // �G���[����
        fprintf(stderr, "error uv_listen");
        return 1;
    }

    auto th = std::thread(
        [&]() {
            uv_run(uv_default_loop(), UV_RUN_DEFAULT);
        });

    for (;;) {

    }

    return 0;
}

#else
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

    IZ_BOOL isRecieved = IZ_FALSE;
    
    client->recieve(
        [&](void* data, IZ_UINT size) {
        IZ_PRINTF("%s(%d)\n", data, size);
        isRecieved = IZ_TRUE;
    });

    while (!isRecieved) {
    }

    IZ_BOOL isClosed = IZ_FALSE;

    client->stop(
        [&](){
        isClosed = IZ_TRUE;
    });

    while (!isClosed) {
    }

    isClosed = IZ_FALSE;

    listener.stop(
        [&](){
        isClosed = IZ_TRUE;
    });

    while (!isClosed) {
    }
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
    client.start(allocator, host);

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

    IZ_BOOL isClosed = IZ_FALSE;

    client.stop(
        [&](){
        isClosed = IZ_TRUE;
    });

    while (!isClosed) {
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