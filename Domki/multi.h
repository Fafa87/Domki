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
void serialize(Archive & archive,
	WymarszRozkaz & m)
{
	archive(CEREAL_NVP(m.skad->uid), CEREAL_NVP(m.dokad->uid), CEREAL_NVP(m.ulamek));
}

namespace multi
{

	// KROKI DO WYKONANIA:
	//+ 1. Ustanowienie komunikacji
	//+ 2. Projekt serwer i projekt klient podpiêci przez tcp i potrafi¹ do siebie pisaæ.
	// 3. Serwer potrafi uruchomiæ Rozgrywkê (widaæ j¹, a nadal mo¿na w konsoli rozmawiaæ), Klient te¿ uruchamia rozgrywkê (ta same ustawienia, ale ustawiaj¹c siebie jako odpowiedniego gracza)
	// 4. Rozgrywka potrafi siê serializowaæ i deserializowaæ. (tu bêdzie trochê tych danych... -> cereal, czy ten drugi?) --> cereal bo tam s¹ cykle z referencji
	// 5. Rozgrywka potrafi wykonaæ podany Rozgrywka::Ruch, który te¿ jest serializowalny i deserializowalny
	// 6. Serwer wysy³a do klientów Rozgrywkê co pewien czas (1/10 sekundy?), Klient uaktualnia j¹ (tzn zastêpuje tym co odebra³).
	// 7. Serwer oczekuje na rozkazy od klientów i aplikuje je do swojej rozgrywki.
	// 8. Sukces??



	// Na serwerze:
	//	Rozgrywka normalna, decydenci stworzeni na podstawie pod³¹czonych ludzi.
	// Na kliencie:
	//  RozgrywkaSluga, poza wykonaniem ruchu przesy³any jest rozkaz do serwera. 

	// 
	// nowa klasa RozgrywkaSerwerowa, gdzie zamiast wykonywaæ ruchy przesy³a siê je do serwera
	// i przyjmuje rzeczywist¹ rozgrywkê z niego 
	// ale to w kliencie, tutaj bêdziemy mieli zwyk³¹ rozgrywkê tylko, ¿e rozkazy bêd¹ przychodziæ ze œmiesznych decydentów
	// s³ychaj¹cych tcp

	// Misji musi siê odbywaæ przez:
	//		Misja<TRozgrywka>, która opisze klasê rozgrywki - lokalna, serwerowa

	// Rozgrywka musi byæ lepiej konfigurowalna (sam proces rozgrywki niezale¿ny od decydentów?) - podczepienie graczy giêtkie

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