#include "multi.h"

using namespace multi;

Adres multi::Serwer::Postaw()
{
	if (nasluchiwacz.listen(85) != sf::Socket::Done)
		printf("listener buraka!");
	return Adres("localhost", nasluchiwacz.getLocalPort());
}

void multi::Serwer::OczekujNaGracza()
{
	Gracz gracz;
	gracz.wtyk = new sf::TcpSocket();
	if (nasluchiwacz.accept(*gracz.wtyk) != sf::Socket::Done)
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
	printf("gracze po³¹czeni w liczbie %d\n", ludzie.size());
}

void multi::Serwer::Start(MisjaUstawienia ustawienia)
{
	for (int i = 0; i < ludzie.size(); i++)
	{
		multi::Wyslij(*ludzie[i].wtyk, "START GRACZ " + to_string(i));
	}
}

void multi::Serwer::Rozeslij(MRozgrywka stan)
{
	for (int i = 0; i < ludzie.size(); i++)
	{
		multi::Wyslij(*ludzie[i].wtyk, stan);
	}
}

vector<MRozkaz> multi::Serwer::Odbierz()
{
	vector<MRozkaz> res;
	for (int i = 0; i < ludzie.size(); i++)
	{
		auto data = multi::Pobierz(*ludzie[i].wtyk);
		if (data.size())
			res.push_back(data[0]);
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

pair<bool, MisjaUstawienia> multi::Klient::OczekujNaStart()
{
	MisjaUstawienia res;

	auto data = multi::Pobierz(*wtyk);
	if (data.size())
	{
		res.nazwa = data[0];
		return { true, res };
	}
	return { false, res };
}


void multi::Klient::Wyslij(MRozkaz rozkaz)
{
	multi::Wyslij(*wtyk, rozkaz);
}


pair<bool, MRozgrywka> multi::Klient::Odbierz()
{
	MRozgrywka res;

	auto data = multi::Pobierz(*wtyk);
	if (data.size())
	{
		res = data[0];
		return { true, res };
	}

	return { false, res };
}


vector<string> multi::Pobierz(sf::TcpSocket& wtyk)
{
	vector<string> res;
	sf::Packet pakiet;
	if (wtyk.receive(pakiet) != sf::Socket::Done)
	{
		//printf("Gracz::Pobierz buraka!");
		return res;
	}

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