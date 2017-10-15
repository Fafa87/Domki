#include "multi_dzialacze.h"


SerwerowyRuszacz::SerwerowyRuszacz(multi::Serwer & serwer) : serwer(serwer)
{
}

void SerwerowyRuszacz::Ruszaj(float czas)
{
	// pobierz ruchy z klient�w
	auto& rozkazy = serwer.Odbierz();
	PrzyjmijRuch(rozkazy);

	Ruszacz::Ruszaj(czas);

	this->czas_od_synchro += czas;
	if (this->czas_od_synchro > this->okres_synchronizacji)
	{
		// rozeslij stan gry do wszystkich klient�w
		serwer.Rozeslij(*rozgrywka);
		this->czas_od_synchro = 0;
	}
}

KlientowyRuszacz::KlientowyRuszacz(multi::Klient & klient) : klient(klient)
{
}

void KlientowyRuszacz::Ruszaj(float czas)
{
	// wy�lij swoje rozkazy
	klient.Wyslij(kolejka_do_wykonania);

	Ruszacz::Ruszaj(czas);

	auto& res = klient.Odbierz();
	if (res.first)
	{
		// uaktualnij rozgrywk� - zast�p t� - przepisuj�c, adres ma zosta� ten sam
		*rozgrywka = res.second;
	}
}