#include "multi.h"

#include <fstream>

using namespace multi;

Adres multi::Serwer::Postaw()
{
	if (nasluchiwacz.listen(85) != sf::Socket::Done)
		printf("listener buraka!");
	return Adres("localhost", nasluchiwacz.getLocalPort());
}

void multi::Serwer::OczekujNaGracza()
{
	Zawodnik gracz;
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
		ustawienia.nr_gracza = i; // TODO uproszczenie
		std::stringstream ss;
		{
			cereal::BinaryOutputArchive archive(ss);
			archive(ustawienia);
		}

		multi::Wyslij(*ludzie[i].wtyk, ss.str());
	}
}

void multi::Serwer::Rozeslij(MRozgrywka& stan)
{
	for (int i = 0; i < ludzie.size(); i++)
	{
		std::stringstream ss;
		{
			cereal::BinaryOutputArchive archive(ss);
			archive(stan);
		}
		multi::Wyslij(*ludzie[i].wtyk, ss.str());
	}
}

vector<Rozkaz*> multi::Serwer::Odbierz()
{
	vector<Rozkaz*> res;
	for (int i = 0; i < ludzie.size(); i++)
	{
		auto data = multi::Pobierz(*ludzie[i].wtyk);
		for (auto &d : data)
		{
			// TODO zak³adamy tutaj, ¿e s¹ tylko rozkazy wymarszu
			WymarszRozkaz* rozkaz = new WymarszRozkaz(nullptr, nullptr);
			std::stringstream ss(d);
			{
				cereal::BinaryInputArchive dearchive(ss);
				dearchive(*rozkaz);
			}

			res.push_back(rozkaz);
		}
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
		std::stringstream ss(data[0]);
		{
			cereal::BinaryInputArchive dearchive(ss);
			dearchive(res);
		}

		return { true, res };
	}
	return { false, res };
}


void multi::Klient::Wyslij(vector<Rozkaz*> rozkazy)
{
	vector<string> dane;
	for (auto r : rozkazy)
	{
		WymarszRozkaz* rozkaz = (WymarszRozkaz*)r;
		std::stringstream ss;
		{
			cereal::BinaryOutputArchive archive(ss);
			archive(*rozkaz);
		}

		dane.push_back(ss.str());
	}

	multi::Wyslij(*wtyk, dane);
}


pair<bool, MRozgrywka> multi::Klient::Odbierz()
{
	MRozgrywka res;

	auto data = multi::Pobierz(*wtyk);
	if (data.size())
	{
		std::stringstream ss(data[0]);
		{
			cereal::BinaryInputArchive archive(ss);
			archive(res);
		}
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

void multi::Podepnij(Rozgrywka& rozgrywka, vector<Rozkaz*> rozkazy)
{
	for (auto r : rozkazy)
	{
		// TODO zaklada ¿e jest tylko rozkaz wymarszu
		WymarszRozkaz * rozkaz = (WymarszRozkaz*)r;
		rozkaz->dokad = rozgrywka.WskaznikDomek(rozkaz->ser_dokad);
		rozkaz->skad = rozgrywka.WskaznikDomek(rozkaz->ser_skad);
	}
}

void multi::Podepnij(Rozgrywka& rozgrywka)
{
	for (auto& r : rozgrywka.domki)
	{
		r.gracz = &rozgrywka.Gracz(r.ser_gracz);
	}

	for (auto& r : rozgrywka.armie)
	{
		r.gracz = &rozgrywka.Gracz(r.ser_gracz);
		r.cel = rozgrywka.WskaznikDomek(r.ser_cel);
	}
}


void multi::Wyslij(sf::TcpSocket& wtyk, vector<string> dane)
{
	sf::Packet pakiet;
	for (auto& d : dane)
		pakiet << d;
	if (wtyk.send(pakiet) != sf::Socket::Done)
		printf("Gracz::Wyslij buraka!");
}

void multi::Wyslij(sf::TcpSocket& wtyk, string dane)
{
	sf::Packet pakiet;
	pakiet << dane;
	if (wtyk.send(pakiet) != sf::Socket::Done)
		printf("Gracz::Wyslij buraka!");
}