#include "define.h"
#include "network.h"
#include <iostream>

Nwork::Nwork() : m_socket(sf::UdpSocket()), m_adress(NW_SERVER_ADDRESS), m_port(NW_PORT)
{
}


Nwork::~Nwork()
{
}

bool Nwork::Send(sf::Packet& pack)
{
	//Test en Udp
	if (m_socket.send(pack, m_adress, m_port) != sf::Socket::Done)
		return false;
	return true;
}
bool Nwork::Receive(sf::Packet& packet)
{
	//Test en Udp
	//Bind du socket (près a recevoir les données)
	m_socket.bind(m_port);
	//Instanciation des variables de l'envoyeur
	sf::Packet pack;
	sf::IpAddress adress;
	unsigned short port;
	//Vérification si le packet a bien été reçu
	if (m_socket.receive(pack, adress, port))
	{
		//S'il a été reçu, assignation du nouveau packet
		packet = pack;
		return true;
	}
	return false;;
}
bool Nwork::StartServer()
{
	//Seulement en TCP
	return true;
}