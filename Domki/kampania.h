#pragma once

#include <vector>
#include <string>

#include "misja.h"

using namespace std;

class Kampania
{
	string nazwa;
	int akt_misja = 1;
	vector<string> lista_misji;
public:
	Kampania(string sciezka);
	~Kampania();

	MisjaUstawienia PobierzMisje(int numer);
};

