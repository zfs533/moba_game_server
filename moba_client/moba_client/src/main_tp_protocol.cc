/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32 // WIN32 宏, Linux宏不存在
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib, "WSOCK32.LIB")
#endif

#include "tp_protocol.h"
*/
/*
int main(int argc, char** argv) {
	int ret;
	// 配置一下windows socket 版本
	// 一定要加上这个，否者低版本的socket会出很多莫名的问题;
#ifdef WIN32
	WORD wVersionRequested;
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
		goto failed;
	}
	// 配置一下要连接服务器的socket
	// 127.0.0.1 本机IP地址;
	struct sockaddr_in sock_addr;
	sock_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(6080); // 连接信息要发送给监听socket;
	// 发送连接请求到我们服务端的监听socket;
	ret = connect((SOCKET)s, (const sockaddr*)&sock_addr, sizeof(sockaddr));
	if (ret != 0) {
		goto failed;
	}

	// 连接成功, s与服务器对应的socket就会建立连接;
	// 客户端在连接的时候他也需要一个IP地址+端口;
	// 端口是服务器端口。不是，客户端一个没有使用的端口就可以了;
	// 客户端自己也会分配一个IP + 端口(只要是没有使用的就可以了);
	// 
	char buf[32];
	memset(buf, 0, 32);

	int pkg_len = 0;
	unsigned char* data = tp_protocol::package((unsigned char*)"Hello", 5, &pkg_len);
	send(s, (const char*)data, pkg_len, 0);
	tp_protocol::release_package(data);
	recv(s, buf, 7, 0);

	int head_size = 0;
	tp_protocol::read_header((unsigned char*)buf, 7, &pkg_len, &head_size);
	printf("%s\n", buf + head_size);

failed:
	if (s != INVALID_SOCKET) {
		closesocket(s);
		s = INVALID_SOCKET;
	}
	
#ifdef WIN32
	WSACleanup();
#endif

	system("pause");
	return 0;
}

*/