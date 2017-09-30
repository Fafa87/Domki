#pragma once

#include <string>
#include <vector>
using namespace std;

// KROKI DO WYKONANIA:
// 1. Ustanowienie komunikacji
// 2. Projekt serwer i projekt klient podpi�ci przez tcp i potrafi� do siebie pisa�.
// 3. Serwer potrafi uruchomi� Rozgrywk� (wida� j�, a nadal mo�na w konsoli rozmawia�), Klient te� uruchamia rozgrywk� (ta same ustawienia, ale ustawiaj�c siebie jako odpowiedniego gracza)
// 4. Rozgrywka potrafi si� serializowa� i deserializowa�. (tu b�dzie troch� tych danych... -> cereal, czy ten drugi?) --> cereal bo tam s� cykle z referencji
// 5. Rozgrywka potrafi wykona� podany Rozgrywka::Ruch, kt�ry te� jest serializowalny i deserializowalny
// 6. Serwer wysy�a do klient�w Rozgrywk� co pewien czas (1/10 sekundy?), Klient uaktualnia j� (tzn zast�puje tym co odebra�).
// 7. Serwer oczekuje na rozkazy od klient�w i aplikuje je do swojej rozgrywki.
// 8. Sukces??

// uruchamianie misji musi si� odbywa� przez:
// Misja<TRozgrywka>, kt�ra opisze klas� rozgrywki
// - lokalna, serwerowa

// 
// nowa klasa RozgrywkaSerwerowa, gdzie zamiast wykonywa� ruchy przesy�a si� je do serwera
// i przyjmuje rzeczywist� rozgrywk� z niego 
// ale to w kliencie, tutaj b�dziemy mieli zwyk�� rozgrywk� tylko, �e rozkazy b�d� przychodzi� ze �miesznych decydent�w
// s�ychaj�cych tcp


// rozgrywka musi by� lepiej konfigurowalna (sam proces rozgrywki niezale�ny od decydent�w?) - podczepienie graczy gi�tkie
// do przeniesienia do misja.h i u�ycia w g��wnym kodzie
struct MisjaUstawienia
{
	string nazwa;
	double szybkosc;
	string trudnosc;
};

struct Adres {
	string ip;
	int port;
};

struct Gracz {
	Adres adres;
	string nazwa;
	// TODO po��czenie TCP
};

class Serwer
{
public:
	Adres Przygotuj();

	// oczekuje graczy na pod��czenie si� i podanie danych, kt�re wyl�duj� w ludzie
	void OczekujNaGraczy(int ilu);

	// za�aduj i odpal misj� z podanym generycznym TRozgrywk�, kt�ry
	void Start(MisjaUstawienia ustawienia); 
private:
	vector<Gracz> ludzie;
};