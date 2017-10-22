#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

#include "dane.h"
#include "rozgrywka.h"

#include "dzialacze.h"
#include "multi.h"


class SerwerowyRuszacz : public Ruszacz
{
public:
	SerwerowyRuszacz(multi::Serwer& serwer);
	virtual void Ruszaj(float czas);
	double okres_synchronizacji = 0.03;

private:
	double czas_od_synchro = 0.0;
	multi::Serwer& serwer;
};

class KlientowyRuszacz : public Ruszacz
{
public:
	KlientowyRuszacz(multi::Klient& klient);
	virtual void Ruszaj(float czas);

private:
	multi::Klient& klient;
};