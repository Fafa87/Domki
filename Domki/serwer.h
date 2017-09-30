#pragma once

#include <string>
#include <vector>

#include "misja.h"

using namespace std;

namespace multi
{

	// KROKI DO WYKONANIA:
	// 1. Ustanowienie komunikacji
	// 2. Projekt serwer i projekt klient podpiêci przez tcp i potrafi¹ do siebie pisaæ.
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
		string ip;
		int port;
	};

	struct Gracz {
		Adres adres;
		string nazwa;
		// TODO po³¹czenie TCP
	};

	class Komunikacja
	{




	};

	class Serwer
	{
	public:
		Adres Przygotuj();

		// oczekuje graczy na pod³¹czenie siê i podanie danych, które wyl¹duj¹ w ludzie
		void OczekujNaGraczy(int ilu);

		// za³aduj i odpal misjê z podanym generycznym TRozgrywkê, który
		void Start(MisjaUstawienia ustawienia);
	private:
		vector<Gracz> ludzie;
	};


	class Klient
	{
	public:
		Klient(string nazwa);

		void Podlacz(Adres serwer);


	};
}