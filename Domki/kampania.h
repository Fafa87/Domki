#pragma once

#include <vector>
#include <string>

#include "misja.h"

using namespace std;

class OpisMisji
{
public:
	OpisMisji(string sciezka);

	wstring powitanie;
	wstring fabula;
	wstring nauka;
};

class Kampania
{
	vector<string> lista_plikow;
public:
	vector<string> lista_misji;
	string nazwa;
	int akt_misja = 1;

	Kampania(string sciezka);
	~Kampania();

	MisjaUstawienia PobierzMisje(int numer);
	OpisMisji PobierzOpis(int numer);
};

