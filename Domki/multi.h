#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Network.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
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

template<class Archive>
void serialize(Archive & archive,
	Rozgrywka & m)
{
	archive(CEREAL_NVP(m.gracze), CEREAL_NVP(m.armie), CEREAL_NVP(m.domki), CEREAL_NVP(m.gracze), CEREAL_NVP(m.liczba_aktywnych_graczy));
}

namespace sf 
{
	template<class Archive>
	void serialize(Archive & archive,
		sf::Color & m)
	{
		archive(m.a, m.b, m.g, m.r);
	}

	template<class Archive>
	void serialize(Archive & archive,
		sf::Vector2<double> & m)
	{
		archive(m.x, m.y);
	}
}

template<class Archive>
void serialize(Archive & archive,
	Gracz & m)
{
	archive(CEREAL_NVP(m.aktywny), CEREAL_NVP(m.liczba_tworow), CEREAL_NVP(m.nazwa), CEREAL_NVP(m.numer), CEREAL_NVP(m.kolor));
}

template<class Archive>
void save(Archive & archive,
	Domek const & m)
{
	archive(m.liczebnosc, m.max_liczebnosc, m.polozenie, m.produkcja, m.rozmiar, m.gracz->numer - 1, m.uid, m.wyglad);
}

template<class Archive>
void load(Archive & archive,
	Domek & m)
{
	archive(m.liczebnosc, m.max_liczebnosc, m.polozenie, m.produkcja, m.rozmiar, m.ser_gracz, m.uid, m.wyglad);
}

template<class Archive>
void save(Archive & archive,
	Ludek const & m)
{
	archive(m.cel->uid, m.gracz->numer - 1, m.liczebnosc, m.polozenie, m.rozmiar, m.uid, m.wyglad);
}

template<class Archive>
void load(Archive & archive,
	Ludek & m)
{
	archive(m.ser_cel, m.ser_gracz, m.liczebnosc, m.polozenie, m.rozmiar, m.uid, m.wyglad);
}


namespace multi
{
	struct Adres {
		Adres() {}
		Adres(string ip, int port) : ip(ip), port(port) {}

		string ip;
		int port;
	};

	struct Zawodnik {
		Adres adres;
		string nazwa;
		sf::TcpSocket* wtyk;
	};

	void Podepnij(Rozgrywka& rozgrywka);
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

		vector<Zawodnik> ludzie;
	private:
		sf::TcpListener nasluchiwacz;
		sf::SocketSelector wtykowiec;
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