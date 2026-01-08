#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

/*
WHAT ALL TO DO FOR CLIENT:
1. Initialize winsock
2. Create a socket
3. Connect to server
4. Send and receive data
5. Close the socket
*/

bool Initialize() {
    WSADATA data;
    return  WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMessage(SOCKET s) {
    std::cout << "Enter your chat name: ";
    std::string name;
    std::getline(std::cin, name);
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        std::string msg = name + ": " + message;
        int bytessent = send(s, msg.c_str(), static_cast<int>(msg.length()), 0);
        if (bytessent == SOCKET_ERROR) {
            std::cout << "Failed to send data." << std::endl;
            break;
        }
        if (message == "exit") {
            std::cout << "Exiting chat." << std::endl;
            break;
        }
    }
}

void ReceiveMessage(SOCKET s) {
    char buffer[1024];
    int bytesreceived;
    std::string msg = "";
    while (true) {
        bytesreceived = recv(s, buffer, static_cast<int>(sizeof(buffer) - 1), 0);
        if (bytesreceived <= 0) {
            std::cout << "Connection closed or error." << std::endl;
            break;
        }
        else {
            msg = std::string(buffer, bytesreceived);
            std::cout << msg << std::endl;
        }
        buffer[bytesreceived] = '\0';
        std::cout << buffer << std::endl;
    }
}

int main() {

    if (!Initialize()) {
        std::cout << "Failed to initialize Winsock." << std::endl;
        return -1;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        std::cout << "Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }

    int port = 12345;
    std::string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server." << std::endl;
        closesocket(s);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server." << std::endl;

    std::thread senderthread([s]() { SendMessage(s); });
    std::thread receiverthread([s]() { ReceiveMessage(s); });

    senderthread.join();
    receiverthread.join();

    closesocket(s);

    WSACleanup();
    return 0;
}