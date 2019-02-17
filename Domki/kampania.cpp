#include "kampania.h"

#include "os.h"

Kampania::Kampania(string sciezka)
{
	this->nazwa = split_parent(sciezka).second;
	this->lista_misji = get_all_names_within_folder(sciezka);
	this->akt_misja = 1;
}

Kampania::~Kampania()
{
}

MisjaUstawienia Kampania::PobierzMisje(int numer)
{
	MisjaUstawienia ustawienia;
	ustawienia.nazwa = this->lista_misji[numer];
	ustawienia.szybkosc = 1;
	ustawienia.trudnosc = "podstawa";
	ustawienia.walka_w_polu = 1;
	ustawienia.do_ilu_wygranych = 1;
	return ustawienia;
}
