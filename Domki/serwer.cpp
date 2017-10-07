#include "serwer.h"



using namespace multi;

Adres multi::Serwer::Przygotuj()
{
	if (listener.listen(85) != sf::Socket::Done)
		printf("listener buraka!");
	return Adres("localhost", listener.getLocalPort());
}

void multi::Serwer::OczekujNaGraczy(int ilu)
{
	while (ludzie.size() < ilu)
	{
		Gracz gracz;

		gracz.wtyk = new sf::TcpSocket();
		if (listener.accept(*gracz.wtyk) != sf::Socket::Done)
		{
			printf("Serwer::OczekujNaGraczy buraka!\n");
		}
		else
		{
			auto nazwa = multi::Pobierz(*gracz.wtyk)[0];
			gracz.adres = Adres(gracz.wtyk->getRemoteAddress().toString(), gracz.wtyk->getRemotePort());
			gracz.nazwa = nazwa;
			printf("polaczylem gracza %s\n", gracz.nazwa.c_str());

			ludzie.emplace_back(gracz);
		}

		Sleep(500);
	}

	printf("gracze po³¹czeni w liczbie %d\n", ilu);
}

void multi::Serwer::Start(MisjaUstawienia ustawienia)
{
	for (int i = 0; i < ludzie.size(); i++)
	{
		multi::Wyslij(*ludzie[i].wtyk, "START GRACZ " + to_string(i));
	}
}

void multi::Serwer::Rozeslij(string dane)
{
	for (int i = 0; i < ludzie.size(); i++)
	{
		multi::Wyslij(*ludzie[i].wtyk, dane);
	}
}

vector<vector<string>> multi::Serwer::Odbierz()
{
	vector<vector<string>> res;
	for (int i = 0; i < ludzie.size(); i++)
	{
		res.push_back(multi::Pobierz(*ludzie[i].wtyk));
	}
	return res;
}

multi::Klient::Klient(string nazwa)
{
	this->nazwa = nazwa;
}

void multi::Klient::Podlacz(Adres serwer)
{
	wtyk = new sf::TcpSocket();
	sf::Socket::Status status = wtyk->connect(serwer.ip, serwer.port);
	if (status != sf::Socket::Done)
	{
		printf("Buraka!\n");
	}
	else
	{
		printf("Polaczony!\n");
		multi::Wyslij(*wtyk, nazwa);
	}
}

vector<string> multi::Pobierz(sf::TcpSocket& wtyk)
{
	vector<string> res;
	sf::Packet pakiet;
	if (wtyk.receive(pakiet) != sf::Socket::Done)
		printf("Gracz::Pobierz buraka!");

	while (pakiet.endOfPacket() == false)
	{
		string tekst;
		pakiet >> tekst;
		res.push_back(tekst);
	}
	return res;
}

void multi::Wyslij(sf::TcpSocket& wtyk, string dane)
{
	sf::Packet pakiet;
	pakiet << dane;
	if (wtyk.send(pakiet) != sf::Socket::Done)
		printf("Gracz::Wyslij buraka!");
}
