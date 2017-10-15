#pragma once

#include <string>
#include <vector>

#include <SFML/Network.hpp>

#include "misja.h"

using namespace std;

namespace multi
{

	// KROKI DO WYKONANIA:
	//+ 1. Ustanowienie komunikacji
	//+ 2. Projekt serwer i projekt klient podpi�ci przez tcp i potrafi� do siebie pisa�.
	// 3. Serwer potrafi uruchomi� Rozgrywk� (wida� j�, a nadal mo�na w konsoli rozmawia�), Klient te� uruchamia rozgrywk� (ta same ustawienia, ale ustawiaj�c siebie jako odpowiedniego gracza)
	// 4. Rozgrywka potrafi si� serializowa� i deserializowa�. (tu b�dzie troch� tych danych... -> cereal, czy ten drugi?) --> cereal bo tam s� cykle z referencji
	// 5. Rozgrywka potrafi wykona� podany Rozgrywka::Ruch, kt�ry te� jest serializowalny i deserializowalny
	// 6. Serwer wysy�a do klient�w Rozgrywk� co pewien czas (1/10 sekundy?), Klient uaktualnia j� (tzn zast�puje tym co odebra�).
	// 7. Serwer oczekuje na rozkazy od klient�w i aplikuje je do swojej rozgrywki.
	// 8. Sukces??



	// Na serwerze:
	//	Rozgrywka normalna, decydenci stworzeni na podstawie pod��czonych ludzi.
	// Na kliencie:
	//  RozgrywkaSluga, poza wykonaniem ruchu przesy�any jest rozkaz do serwera. 

	// 
	// nowa klasa RozgrywkaSerwerowa, gdzie zamiast wykonywa� ruchy przesy�a si� je do serwera
	// i przyjmuje rzeczywist� rozgrywk� z niego 
	// ale to w kliencie, tutaj b�dziemy mieli zwyk�� rozgrywk� tylko, �e rozkazy b�d� przychodzi� ze �miesznych decydent�w
	// s�ychaj�cych tcp

	// Misji musi si� odbywa� przez:
	//		Misja<TRozgrywka>, kt�ra opisze klas� rozgrywki - lokalna, serwerowa

	// Rozgrywka musi by� lepiej konfigurowalna (sam proces rozgrywki niezale�ny od decydent�w?) - podczepienie graczy gi�tkie

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

	typedef Rozgrywka MRozgrywka;
	typedef Rozkaz MRozkaz;

	class Serwer
	{
	public:
		Adres Postaw();

		// oczekuje na pod��czenie si� gracza i podanie danych
		void OczekujNaGracza();

		// wy�lij info o starcie do graczy
		void Start(MisjaUstawienia ustawienia);
		void Rozeslij(MRozgrywka& stan);
		vector<MRozkaz*> Odbierz();

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
		void Wyslij(vector<MRozkaz*> rozkazy);
		pair<bool, MRozgrywka> Odbierz();

		string nazwa;
		sf::TcpSocket* wtyk;
	};
}