#include "multi_dzialacze.h"


SerwerowyRuszacz::SerwerowyRuszacz(multi::Serwer & serwer) : serwer(serwer)
{
}

void SerwerowyRuszacz::Ruszaj(double czas)
{
	// pobierz ruchy z klientów
	auto& rozkazy = serwer.Odbierz();
	multi::Podepnij(*rozgrywka, rozkazy);
	PrzyjmijRuch(rozkazy);

	Ruszacz::Ruszaj(czas);

	this->czas_od_synchro += czas;
	if (this->czas_od_synchro > this->okres_synchronizacji)
	{
		// rozeslij stan gry do wszystkich klientów
		serwer.Rozeslij(*rozgrywka);
		this->czas_od_synchro = 0;
	}
}

KlientowyRuszacz::KlientowyRuszacz(multi::Klient & klient) : klient(klient)
{
}

void KlientowyRuszacz::Ruszaj(double czas)
{
	// wyœlij swoje rozkazy
	klient.Wyslij(kolejka_do_wykonania);

	//kolejka_do_wykonania.clear();
	Ruszacz::Ruszaj(czas);

	auto& res = klient.Odbierz();
	if (res.first)
	{
		// uaktualnij rozgrywkê - zast¹p t¹ - przepisuj¹c, adres ma zostaæ ten sam
		*rozgrywka = res.second;
		multi::Podepnij(*rozgrywka);
	}
}
