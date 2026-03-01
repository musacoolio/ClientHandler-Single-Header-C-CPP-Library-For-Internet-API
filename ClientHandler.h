#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#ifdef _WIN32 
#include<WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif
using namespace std;
struct PacketHandle {
	vector<uint8_t> sendLine;
	vector<uint8_t> receiveLine;
	SOCKET CurrentSocket;
	uint16_t port;
	char IP[256] = "127.0.0.1";
	std::mutex receiveMutex;
	std::atomic<bool> SocketState{ false };
	void Connect();
	void SendPacket();
	void ReceivePacket();
	void StringWrite(std::string& OutStream);
};
#ifdef NETWORK_LIB_START
void PacketHandle::ReceivePacket() {
	std::lock_guard<std::mutex> lock(receiveMutex);
	int bytes = recv(CurrentSocket, reinterpret_cast<char*>(receiveLine.data()), receiveLine.size(), 0);
	if (bytes > 0) return;
	SocketState.store(false);
}
void PacketHandle::SendPacket() {
	send(CurrentSocket, reinterpret_cast<char*>(sendLine.data()), sendLine.size(), 0);
}
void PacketHandle::Connect() {
#ifdef _WIN32
	WSADATA wsaver;
	WSAStartup(MAKEWORD(2, 2), &wsaver);
#endif
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG;
	addrinfo* result = nullptr;
	int res = getaddrinfo(IP, to_string(port).c_str(), &hints, &result);
	if (res != 0) return;
ConnectAttempt:
	CurrentSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	int status = connect(CurrentSocket, result->ai_addr, result->ai_addrlen);
	if (status == 0) SocketState.store(true);
	if (status < 0) {SocketState.store(false);closesocket(CurrentSocket);}
	if (!SocketState.load()) goto ConnectAttempt;
	freeaddrinfo(result);
}
void PacketHandle::StringWrite(std::string& OutStream) {
	ReceivePacket();
	std::lock_guard<std::mutex> lock(receiveMutex);
	if (!receiveLine.empty()) OutStream = string(receiveLine.begin(), receiveLine.end());
}
#endif
