#include "MultiClientListener.h"
#include <string>

#include "Database.h"

Database db("db.txt");

// Handler for client connections
void MultiClientListener::onClientConnected(int clientSocket)
{
	std::cout << "Client with socket #" << clientSocket << " connected." << std::endl;
}

// Handler for client disconnections
void MultiClientListener::onClientDisconnected(int clientSocket)
{
	std::cout << "Client with socket #" << clientSocket << " disconnected." << std::endl;
}

void MultiClientListener::onMessageReceived(int sock, const char* msg, int bytesIn)
{
	std::cout << std::string(msg, 0, bytesIn) << std::endl;
	if (db.checkLoginPassword(msg))
	{
		std::cout << "Client entered wrong details." << std::endl;
		sendToClient(sock, "Wrong username or password.");
	}
	else
	{
		std::cout << "Client logged in!" << std::endl;
		sendToClient(sock, "Logged in!");
	}
}