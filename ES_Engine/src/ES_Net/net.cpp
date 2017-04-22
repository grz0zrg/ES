#include "ES_Net/net.h"

namespace ES
{
	ES_Network::ES_Network()
	{

	}

	ES_Network::~ES_Network()
	{

	}

	int ES_Network::init(short int mode, char *host, Uint16 port)
	{
		if(SDLNet_Init() < 0) return -1;
		if(mode == ES_UDP)
		{
			socketUDP = SDLNet_UDP_Open(port);
			if(!socketUDP) return -1;
		}
		else
		{
			if(SDLNet_ResolveHost(&ip, host, port) < 0) return -2;
			socketTCP=SDLNet_TCP_Open(&ip);
			if(!socketTCP) return -1;
		}
		return 0;
	}

	void ES_Network::quit()
	{
		if(socketUDP) SDLNet_UDP_Close(socketUDP);
		if(socketTCP) SDLNet_TCP_Close(socketTCP);
		socketUDP = NULL;
		socketTCP = NULL;

		SDLNet_Quit();
	}

	TCPsocket ES_Network::getTCP_Socket()
	{
		return socketTCP;
	}

	UDPsocket ES_Network::getUDP_Socket()
	{
		return socketUDP;
	}

	std::string ES_Network::getError()
	{
		return SDLNet_GetError();
	}

	SDLNet_SocketSet ES_Network::allocSocketSet(int max_socks)
	{
		s_set = SDLNet_AllocSocketSet(max_socks);

		return s_set;
	}

	SDLNet_SocketSet ES_Network::allocSocketSet(SDLNet_SocketSet socket_set, int max_socks)
	{
		socket_set = SDLNet_AllocSocketSet(max_socks);

		return socket_set;
	}

	void ES_Network::freeSocketSet()
	{
		SDLNet_FreeSocketSet(s_set);
	}

	void ES_Network::freeSocketSet(SDLNet_SocketSet socket_set)
	{
		SDLNet_FreeSocketSet(socket_set);
	}

	int ES_Network::addUDP_Socket(UDPsocket sock)
	{
		return SDLNet_UDP_AddSocket(s_set, sock);
	}

	int ES_Network::addTCP_Socket(TCPsocket sock)
	{
		return SDLNet_TCP_AddSocket(s_set, sock);
	}

	int ES_Network::addUDP_Socket(SDLNet_SocketSet set, UDPsocket sock)
	{
		return SDLNet_UDP_AddSocket(set, sock);
	}

	int ES_Network::addTCP_Socket(SDLNet_SocketSet set, TCPsocket sock)
	{
		return SDLNet_TCP_AddSocket(set, sock);
	}

	int ES_Network::sendUDP(UDPpacket *src)
	{
		return SDLNet_UDP_Send(socketUDP, -1, src);
	}

	int ES_Network::recvUDP(UDPpacket *dst)
	{
		return SDLNet_UDP_Recv(socketUDP, dst);
	}

	int ES_Network::sendTCP(void *data, int len)
	{
		return SDLNet_TCP_Send(socketTCP, data, len);
	}

	int ES_Network::sendTCP(TCPsocket socket, void *data, int len)
	{
		return SDLNet_TCP_Send(socket, data, len);
	}

	int ES_Network::recvTCP(void *data, int maxlen)
	{
		return SDLNet_TCP_Recv(socketTCP, data, maxlen);
	}

	TCPsocket ES_Network::acceptConnection()
	{
		return SDLNet_TCP_Accept(socketTCP);
	}
}
