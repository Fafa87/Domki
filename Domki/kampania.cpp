#include "kampania.h"

#include <locale>
#include <codecvt>
#include <string>

#include "os.h"


Kampania::Kampania(string sciezka)
{
	this->nazwa = split_parent(sciezka).second;
	this->lista_plikow = get_all_names_within_folder(sciezka, false, true);
	for (int i = 0; i < this->lista_plikow.size() / 2; i++)
		this->lista_misji.push_back(this->lista_plikow[i]);
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
	ustawienia.szybkosc = 1.3;
	ustawienia.trudnosc = "podstawa";
	ustawienia.walka_w_polu = 1;
	ustawienia.do_ilu_wygranych = 1;
	return ustawienia;
}

OpisMisji Kampania::PobierzOpis(int numer)
{
	return OpisMisji(this->lista_plikow[numer - 1 + this->lista_plikow.size() / 2]);
}

OpisMisji::OpisMisji(string sciezka)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	ifstream plik;

	std::ios_base::iostate exceptionMask = plik.exceptions() | std::ios::failbit;
	plik.exceptions(exceptionMask);

	try {
		plik.open(sciezka);
	}
	catch (std::ios_base::failure& e) {
		std::cout << "Caught an ios_base::failure.\n"
			<< "Explanatory string: " << e.what() << '\n'
			<< "Error code: " << e.code() << '\n';

		std::fflush(stdout);
	}

	//cerr << "Error: " << strerror(errno);

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
