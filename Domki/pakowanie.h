#pragma once

#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>

#include "misja.h"

using namespace std;

template<class Archive>
void serialize(Archive & archive,
	MisjaUstawienia & m)
{
	// TODO szybkosc nie przesyla siê
	archive(CEREAL_NVP(m.nazwa), CEREAL_NVP(m.skorka), CEREAL_NVP(m.grupa), CEREAL_NVP(m.szybkosc), CEREAL_NVP(m.walka_w_polu), CEREAL_NVP(m.trudnosc),
		CEREAL_NVP(m.nr_gracza), CEREAL_NVP(m.nazwy_graczow), CEREAL_NVP(m.komputery),
		CEREAL_NVP(m.do_ilu_wygranych), CEREAL_NVP(m.ile_kto_wygranych));
}

// MULTI ROZKAZ
template<class Archive>
void save(Archive & archive,
	WymarszRozkaz const & m)
{
	archive(m.skad->uid, m.dokad->uid, CEREAL_NVP(m.ulamek));
}

template<class Archive>
void load(Archive & archive,
	WymarszRozkaz & m)
{
	archive(m.ser_skad, m.ser_dokad, CEREAL_NVP(m.ulamek));
}

template<class Archive>
void save(Archive & archive,
	UlepszRozkaz const & m)
{
	archive(m.kogo->uid);
}

template<class Archive>
void load(Archive & archive,
	UlepszRozkaz & m)
{
	archive(m.ser_kogo);
}

template<class Archive>
void save(Archive & archive,
	PrzebudujRozkaz const & m)
{
	archive(m.kogo->uid, m.naco);
}

template<class Archive>
void load(Archive & archive,
	PrzebudujRozkaz & m)
{
	archive(m.ser_kogo, m.naco);
}

template<class Archive>
void save(Archive & archive,
	BurzRozkaz const & m)
{
	archive(m.kogo->uid);
}

template<class Archive>
void load(Archive & archive,
	BurzRozkaz & m)
{
	archive(m.ser_kogo);
}

template<class Archive>
void serialize(Archive & archive,
	Rozgrywka & m)
{
	archive(CEREAL_NVP(m.walka_w_polu), CEREAL_NVP(m.gracze), CEREAL_NVP(m.armie), CEREAL_NVP(m.domki), CEREAL_NVP(m.liczba_aktywnych_graczy));
}

namespace sf
{
	template<class Archive>
	void serialize(Archive & archive,
		sf::Color & m)
	{
		archive(m.a, m.b, m.g, m.r);
	}

	template<class Archive>
	void serialize(Archive & archive,
		sf::Vector2<double> & m)
	{
		archive(m.x, m.y);
	}
}

// ROZGRYWKA

template<class Archive>
void serialize(Archive & archive,
	Gracz & m)
{
	archive(CEREAL_NVP(m.aktywny), CEREAL_NVP(m.liczba_tworow), CEREAL_NVP(m.nazwa), CEREAL_NVP(m.numer), CEREAL_NVP(m.kolor), CEREAL_NVP(m.istotny));
}

template<class Archive>
void save(Archive & archive,
	Domek const & m)
{
	vector<int> ser_drogi;
	for (auto d : m.drogi)
		ser_drogi.push_back(d->uid);
	archive(m.typdomku, m.poziom, m.liczebnosc, m.max_liczebnosc, m.polozenie, m.produkcja, m.rozmiar, m.gracz->numer, m.uid, m.wyglad, ser_drogi);
}

template<class Archive>
void load(Archive & archive,
	Domek & m)
{
	archive(m.typdomku, m.poziom, m.liczebnosc, m.max_liczebnosc, m.polozenie, m.produkcja, m.rozmiar, m.ser_gracz, m.uid, m.wyglad, m.ser_drogi);
}

template<class Archive>
void save(Archive & archive,
	Ludek const & m)
{
	archive(m.tarcza, m.szybkosc_ludka, m.cel->uid, m.gracz->numer, m.liczebnosc, m.polozenie, m.rozmiar, m.uid, m.wyglad);
}

template<class Archive>
void load(Archive & archive,
	Ludek & m)
{
	archive(m.tarcza, m.szybkosc_ludka, m.ser_cel, m.ser_gracz, m.liczebnosc, m.polozenie, m.rozmiar, m.uid, m.wyglad);
}
