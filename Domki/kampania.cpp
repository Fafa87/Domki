#include "kampania.h"

#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

#include "os.h"
#include "ext_string.h"


Kampania::Kampania(string sciezka)
{
	this->nazwa = split_parent(sciezka).second;
	this->lista_plikow = get_all_names_within_folder(sciezka, false, true);
	std::copy_if(lista_plikow.begin(), lista_plikow.end(), std::back_inserter(this->lista_misji), 
		[](string path) { return ends_with(path, ".txt"); });
	std::copy_if(lista_plikow.begin(), lista_plikow.end(), std::back_inserter(this->lista_opisow),
		[](string path) { return ends_with(path, ".ops"); });

	this->akt_misja = 1;
}

Kampania::~Kampania()
{
}

MisjaUstawienia Kampania::PobierzMisje(int numer)
{
	MisjaUstawienia ustawienia;
	ustawienia.grupa = "";
	ustawienia.nazwa = this->lista_misji[numer - 1];
	ustawienia.szybkosc = 1.5;
	ustawienia.trudnosc = "pokojowy";
	ustawienia.walka_w_polu = 1;
	ustawienia.do_ilu_wygranych = 1;
	return ustawienia;
}

OpisMisji Kampania::PobierzOpis(int numer)
{
	return OpisMisji(this->lista_opisow[numer - 1]);
}

string Kampania::PobierzOdprawe(int numer)
{
	string sciezka = this->lista_misji[numer - 1];
	replace_ext(sciezka, "ogg");

	if (std::experimental::filesystem::exists(sciezka))
		return sciezka;
	else
		return string();
}

OpisMisji::OpisMisji(string sciezka)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	ifstream plik;
	plik.open(sciezka);

	string tmp;
	std::getline(plik, tmp);
	this->powitanie = converter.from_bytes(tmp);

	tmp = "";
	string linia;
	while (std::getline(plik, linia))
	{
		if (linia == "\n" || linia =="")
			break;
		tmp += linia + "\n";
	}
	this->fabula = converter.from_bytes(tmp);

	tmp = "";
	while (!plik.eof() && std::getline(plik, linia))
	{
		if (linia == "\n" || linia == "")
			break;
		tmp += linia + "\n";
	}
	this->nauka = converter.from_bytes(tmp);
	plik.close();
}
