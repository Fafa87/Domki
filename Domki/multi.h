#pragma once

#include <string>
#include <vector>

#include <SFML/Network.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

#include "misja.h"

using namespace std;

template<class Archive>
void serialize(Archive & archive,
	MisjaUstawienia & m)
{
	archive(CEREAL_NVP(m.nazwa), CEREAL_NVP(m.szybkosc), CEREAL_NVP(m.trudnosc), CEREAL_NVP(m.nr_gracza), CEREAL_NVP(m.nazwy_graczow), CEREAL_NVP(m.komputery));
}

template<class Archive>
void save(Archive & archive,
	WymarszRozkaz const & m)
{
	archive(m.skad->uid, m.dokad->uid, CEREAL_NVP(m.ulamek));
}

template<class Archive>
void load(Archive & archive,
	WymarszRozkaz & m)
{
	archive(m.ser_skad, m.ser_dokad, CEREAL_NVP(m.ulamek));
}

namespace multi
{
	struct Adres {
		Adres() {}
		Adres(string ip, int port) : ip(ip), port(port) {}

		string ip;
		int port;
	};

	struct Gracz {
		Adres adres;
		string nazwa;
		sf::TcpSocket* wtyk;
	};


	void Podepnij(Rozgrywka& rozgrywka, vector<Rozkaz*> rozkazy);
	vector<string> Pobierz(sf::TcpSocket& wtyk);
	void Wyslij(sf::TcpSocket& wtyk, string dane);
	void Wyslij(sf::TcpSocket& wtyk, vector<string> dane);

	typedef Rozgrywka MRozgrywka;

	class Serwer
	{
	public:
		Adres Postaw();

		// oczekuje na pod³¹czenie siê gracza i podanie danych
		void OczekujNaGracza();

		// wyœlij info o starcie do graczy
		void Start(MisjaUstawienia ustawienia);
		void Rozeslij(MRozgrywka& stan);
		vector<Rozkaz*> Odbierz();

		vector<Gracz> ludzie;
	private:
		sf::TcpListener nasluchiwacz;
	};

	class Klient
	{
	public:
		Klient(string nazwa);

		void Podlacz(Adres serwer);

		pair<bool, MisjaUstawienia> OczekujNaStart();
		void Wyslij(vector<Rozkaz*> rozkazy);
		pair<bool, MRozgrywka> Odbierz();

		string nazwa;
		sf::TcpSocket* wtyk;
	};
}