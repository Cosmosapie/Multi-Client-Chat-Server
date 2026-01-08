#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // for inet_pton
#include <tchar.h> // for _T macro
#include <thread>
#include <vector>
#include <functional>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // link winsock library

/*
WHAT ALL TO DO IN THIS FILE FOR SERVER:
1. Initialize winsock library
2. Create the socket
3. Get IP and Port
4. Bind the IP/Port with the socket
5. Listen on the socket
6. Accept
7. Recv and Send data
8. Close the socket
9. Cleanup winsock
*/

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients) {
	// Send a message to the client
	cout << "Client connected!" << endl;
	char buffer[4096];

	while (1) {
		int bytesrecvd = recv(clientSocket, buffer, static_cast<int>(sizeof(buffer)), 0);

		if (bytesrecvd == 0) {
			cout << "Client disconnected" << endl;
			break;
		}

		string message(buffer, bytesrecvd);
		cout << "Message from client: " << message << endl;

		for (auto client : clients) {
			if (client != clientSocket) // don't send the message back to the sender
				send(client, message.c_str(), message.length(), 0);
		}
	}

	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}

	closesocket(clientSocket);

}

int main() {
	if (!Initialize()) {
		cout << "Failed to initialize winsock" << endl;
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4, AF_INET6 for IPv6, SOCK_STREAM for TCP, SOCK_DGRAM for UDP, 0 - service provider decides what protocol to use
	if (listenSocket == INVALID_SOCKET) {
		cout << "Failed to create socket" << endl;
		WSACleanup();
		return -1;
	}

	// Create address structure
	int port = 12345; // Port number
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET; // IPv4
	serveraddr.sin_port = htons(port);

	// Convert IP address from string to binary and put it inside sin_family
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << "Failed to convert IP address" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Bind
	if (::bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), static_cast<int>(sizeof(serveraddr))) == SOCKET_ERROR) {
		cout << "Bind failed with error: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// Listen
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) { // SOMAXCONN - maximum number of connections
		cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server is listening on port: " << port << endl;
	vector<SOCKET> clients;
	while (1) {
		// Accept
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Accept failed with error: " << WSAGetLastError() << endl;
			closesocket(listenSocket);
			WSACleanup();
			return -1;
		}

		clients.push_back(clientSocket);
		thread t1(InteractWithClient, clientSocket, ref(clients));
		t1.detach(); // Detach the thread to allow it to run independently
	}

	closesocket(listenSocket);

	WSACleanup();
	return 0;
}