#include <iostream>
#include <string>
#include <SFML/Network.hpp>

void StartServer()
{
	sf::IpAddress serverAdress = sf::IpAddress::getLocalAddress();

	sf::TcpListener Listener;

	if (!Listener.listen(55001))
	{
		std::cout << "Erreur lors de l'ouverture du port" << std::endl;
		return;
	}
	std::cout << "Adresse IP du serveur: " << serverAdress.toString() << std::endl;
	std::cout << "Sur ecoute..." << std::endl;

	sf::IpAddress clientAdress;
	sf::TcpSocket Client;
	if (Listener.accept(Client) != sf::Socket::Done)
	{
		std::cout << "En attente de connexion..." << std::endl;
	}

	std::cout << "Un client s'est connecte" << std::endl;

	sf::Packet packet;
	packet << "Je suis le serveur";
	if (Client.send(packet) != sf::Socket::Done)
		return;
	std::cout << "Paquet envoye aux clients" << std::endl;

	// Reception du paquet
	sf::Packet packetRecu;
	if (Client.receive(packetRecu) != sf::Socket::Done)
		return;

	// ouverture du paquet
	std::string message;
	packetRecu >> message;
	std::cout << "Message recu par le client : " << message << std::endl;

	Listener.close();
	//Client.close();
}

void ExecuterClient()
{
	sf::TcpSocket Client;
	if (!Client.connect(sf::IpAddress("127.0.0.1"), 10512))
		std::cout << "Erreur lors de la connexion au serveur" << std::endl;

	sf::Packet packet;
	packet << "Salut toi";
	if (Client.send(packet) != sf::Socket::Done)
		std::cout << "Erreur lors de l'envoie du message" << std::endl;

	// Reception du paquet
	sf::Packet packetRecu;
	if (Client.receive(packetRecu) != sf::Socket::Done)
		return;

	// ouverture du paquet
	std::string message;
	packetRecu >> message;
	std::cout << "Message recu par le serveur : " << message << std::endl;

	//Client.close();
}

int main()
{
	char reponse;
	std::cout << "Etre le serveur (s) ou le client (c) ?" << std::endl;
	std::cin >> reponse;

	if (reponse == 's')
		StartServer();
	else ExecuterClient();

	std::cout << "Enter pour quitter" << std::endl;
	std::cin.ignore(10000, '\n');
	std::cin.ignore(10000, '\n');
	return EXIT_SUCCESS;
}