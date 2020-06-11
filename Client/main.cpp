#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

void main()
{
	std::string serverIP = "127.0.0.1";
	int port = 54000;				// listening port on the server

	// Initialize winsock

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Can't initialize winsock!" << std::endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError()<< std::endl;
		WSACleanup();
		return;
	}

	// What server and what port we want to connect to

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	inet_pton(AF_INET, serverIP.c_str(), &server.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&server, sizeof(server));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Send and receive data
	char buf[BUFFER_SIZE];
	std::string login;
	std::string pass;

	do
	{
		std::cout << "Login: ";
		std::cin >> login;
		std::cout << "Password: ";
		std::cin >> pass;
		std::string loginPass = login + " " + pass;

		if (loginPass.size() > 0)
		{
			int sendResult = send(sock, loginPass.c_str(), loginPass.size() + 1, 0);
			// size() + 1 because in C every string ends with 0 character
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, BUFFER_SIZE);
				int bytesReceived = recv(sock, buf, BUFFER_SIZE, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					std::string response = std::string(buf, 0, bytesReceived);
					std::cout << "SERVER> " << response << std::endl;

					// Check if logged in
					if (!response.compare("Logged in!"))
					{
						break;
					}
				}
			}
		}

	} while (login.size() > 0);

	closesocket(sock);
	WSACleanup();
}