#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#ifdef _WIN32 
#include<WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
typedef int SOCKET;
#endif
using namespace std;

struct PacketHandle {
	vector<uint8_t> sendLine;
	vector<uint8_t> receiveLine;
	SOCKET CurrentSocket;
	uint16_t port;
	char IP[16] = "127.0.0.1";
	std::mutex receiveMutex;
	std::atomic<bool> SocketState{ false };
	void Connect();
	void SendPacket();
	void ReceivePacket();
	void MessageReceive(std::string& OutStream);
};
#ifdef NETWORK_LIB_START

void PacketHandle::ReceivePacket() {
	std::vector<uint8_t> temp; temp.resize(8080);
#ifdef _WIN32
	int bytes = recv(CurrentSocket, reinterpret_cast<char*>(temp.data()), temp.size(), 0);
	if (bytes > 0) {
		std::lock_guard<std::mutex> lock(receiveMutex);
		receiveLine.assign(temp.begin(), temp.begin() + bytes);
		return;
	}
	if (bytes == 0) {
		SocketState.store(false);
		std::lock_guard<std::mutex> lock(receiveMutex);
		receiveLine.clear();
		return;
	}
	if (bytes < 0) {
		SocketState.store(false);
		return;
	}
	SocketState.store(false);
	std::lock_guard<std::mutex> lock(receiveMutex);
	receiveLine.clear();
#else
	int bytes = recv(CurrentSocket, reinterpret_cast<char*>(temp.data()), temp.size(), 0);
	if (bytes > 0) {
		std::lock_guard<std::mutex> lock(receiveMutex);
		receiveLine.assign(temp.begin(), temp.begin() + bytes);
		return;
	}
	if (bytes == 0) {
		SocketState.store(false);
		std::lock_guard<std::mutex> lock(receiveMutex);
		receiveLine.clear();
		return;
	}
	if (bytes < 0) {
		SocketState.store(false);
		return;
	}
	SocketState.store(false);
	std::lock_guard<std::mutex> lock(receiveMutex);
	receiveLine.clear();
#endif 
}

void PacketHandle::SendPacket() {
#ifdef _WIN32
	send(CurrentSocket, reinterpret_cast<char*>(sendLine.data()), sendLine.size(), 0);
#else
	send(CurrentSocket, reinterpret_cast<char*>(sendLine.data()), sendLine.size(), 0);
#endif
}

void PacketHandle::Connect() {
#ifdef _WIN32
	WSADATA wsaver;
	WSAStartup(MAKEWORD(2, 2), &wsaver);
	CurrentSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, static_cast<char*>(IP), &addr.sin_addr);
	ConnectAttempt:
	int status = connect(CurrentSocket, (sockaddr*)&addr, sizeof(addr));
	if (status < 0) {
		SocketState.store(false);
	}
	while (!SocketState.load()) { cout << "\033[HWaiting For Server..."; this_thread::sleep_for(chrono::milliseconds(1)); goto ConnectAttempt;}
	SocketState.store(true);
#else
	CurrentSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, static_cast<char*>(IP), &addr.sin_addr);
	ConnectAttempt:
	int status = connect(CurrentSocket, (sockaddr*)&addr, sizeof(addr));
	if (status < 0) {
		SocketState.store(false);
	}
	while (!SocketState.load()) { cout << "\033[HWaiting For Server..."; this_thread::sleep_for(chrono::milliseconds(1)); goto ConnectAttempt;}
	SocketState = true;
#endif
}

void PacketHandle::MessageReceive(std::string& OutStream) {
	ReceivePacket();
	if (!receiveLine.empty()) {
		OutStream = string(receiveLine.begin(), receiveLine.end());
	}
}
#endif 