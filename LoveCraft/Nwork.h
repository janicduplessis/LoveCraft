#ifndef NWORK_H_
#define NWORK_H_

#include <SFML/Network.hpp>

class Nwork
{
public:
	Nwork();
	~Nwork();
	bool Send(sf::Packet& pack);
	bool Receive(sf::Packet& pack);
	bool StartServer();
private:
	sf::UdpSocket m_socket;
	sf::IpAddress m_adress;
	unsigned short m_port;
};

#endif