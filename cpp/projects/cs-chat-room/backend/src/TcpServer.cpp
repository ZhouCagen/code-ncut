#include <TcpServer.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

TcpServer::TcpServer(std::uint16_t serverPort)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::run()
{
}

void TcpServer::createSocket()
{
}
void TcpServer::setReuseAddress()
{
}
void TcpServer::bindAddress()
{
}
void TcpServer::listenSocket()
{
}
void TcpServer::acceptLoop()
{
}
void TcpServer::handleClient(int clientSocket)
{
}
