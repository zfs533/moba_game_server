#include <string.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
using namespace std;

//protobuf lib
#pragma comment(lib,"libprotobufd.lib")
#pragma comment(lib,"libprotocd.lib")


#include "../proto/game.pb.h"

#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib, "WSOCK32.LIB")


#include "tp_protocol.h"
int main(int argc, char** argv) {
	int ret;

#ifdef WIN32
	DWORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0) {
		printf("WSAStart up failed\n");
		system("pause");
		return -1;
	}
#endif

	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		return 0;
	}

	struct sockaddr_in sockaddr;
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(6080); 
	ret = connect((SOCKET)s, (const struct sockaddr*)&sockaddr, sizeof(sockaddr));
	if (ret != 0) {
		return 0;
	}

	LoginReq req;
	req.set_age(10);
	req.set_name("blake");
	req.set_email("blake@cuby.com");

	int len = req.ByteSize();
	char* data = (char*)malloc(8 + len);
	memset(data, 0, 8 + len);
	req.SerializePartialToArray(data + 8, len);

	int pkg_len;
	unsigned char* pkg_data = tp_protocol::package((unsigned char*)data, 8 + len, &pkg_len);
	send(s, (const char*)pkg_data, pkg_len, 0);

	free(data);
	tp_protocol::release_package(pkg_data);

	unsigned char recv_buf[256];
	int recv_len = recv(s, (char*)recv_buf, 256, 0);

	int pkg_size, header_size;

	tp_protocol::read_header(recv_buf, recv_len, &pkg_size, &header_size);
	/*
	if (s != INVALID_SOCKET) {
		closesocket(s);
		s = INVALID_SOCKET;
	}
	*/
	req.ParseFromArray(recv_buf + header_size + 8, pkg_size - header_size - 8);
	printf("%s: %d\n", req.email().c_str(), req.age());
	/*
#ifdef WIN32
	WSACleanup();
#endif
	*/
	system("pause");
	return 0;
}

