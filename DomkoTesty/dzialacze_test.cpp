#include "CppUnitTest.h"
#include <string>

#include "../Domki/rozgrywka.h"
#include "../Domki/dzialacze.h"
#include "../Domki/os.h"

#include "tworca.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DomkoTesty
{
	TEST_CLASS(DzialaczeTest)
	{
		Rozgrywka rozgrywka;
		Ruszacz ruszacz;
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			rozgrywka = Rozgrywka();
			ruszacz = Ruszacz();
			ruszacz.rozgrywka = &rozgrywka;
		}

		TEST_METHOD(ProstaWalka_Przegrana)
		{
			auto& domek_0 = tworca::DodajDomek(rozgrywka, 0, 101, PD());
			auto& armia_atak = tworca::DodajLudka(rozgrywka, domek_0, 1, 32, PD());

			Assert::AreEqual(101.0, domek_0.liczebnosc);
			ruszacz.Ruszaj(10); // brak produkcja
			Assert::AreEqual(69.0, domek_0.liczebnosc);

			domek_0.produkcja = 2;
			ruszacz.Ruszaj(10); // brak produkcja
			Assert::AreEqual(89.0, domek_0.liczebnosc);
		}

		TEST_METHOD(ludki_sie_nie_mijaja)
		{
			auto& domek_0 = tworca::DodajDomek(rozgrywka, 0, 100, PD(1, 1));
			auto& domek_1 = tworca::DodajDomek(rozgrywka, 1, 100, PD(5, 5));
			auto& armia_atak = tworca::DodajLudka(rozgrywka, domek_0, 1, 99, PD(1,1));
			auto rozkaz = new WymarszRozkaz(&domek_0, &domek_1);
			rozkaz->ulamek = 0.5;
			vector<Rozkaz*> rozkazy = { rozkaz };
			ruszacz.PrzyjmijRuch(rozkazy);
			ruszacz.Ruszaj(10);
			Assert::AreNotEqual(domek_0.gracz->numer, domek_1.gracz->numer);
			Assert::AreEqual(domek_0.liczebnosc, 1.0);
		}

		TEST_METHOD(LudekMalyNieMinie)
		{
			auto& domek_0 = tworca::DodajDomek(rozgrywka, 0, 100, PD(1, 1));
			auto& domek_1 = tworca::DodajDomek(rozgrywka, 1, 100, PD(50, 50));
			auto& armia_probny = tworca::DodajLudka(rozgrywka, domek_1, 0, 1, PD(50, 50));
			auto rozmiar_1 = armia_probny.rozmiar;
			auto rozmiar_domku = domek_0.rozmiar;
			auto pomiedzy = (rozmiar_domku + rozmiar_1) / 2;

			Assert::AreEqual(true, rozmiar_1 < rozmiar_domku);

			auto& armia_atak = tworca::DodajLudka(rozgrywka, domek_0, 1, 99, PD(1 + pomiedzy, 1));
			Assert::AreEqual(true, pomiedzy < min(rozmiar_domku, armia_atak.rozmiar));

			auto rozkaz = new WymarszRozkaz(&domek_0, &domek_1);
			rozkaz->ulamek = 0.01; // jeden ludek
			vector<Rozkaz*> rozkazy = { rozkaz };
			ruszacz.PrzyjmijRuch(rozkazy);
			ruszacz.Ruszaj(0.001);
			Assert::AreNotEqual(domek_0.gracz->numer, domek_1.gracz->numer);
			Assert::AreEqual(99.0, domek_1.liczebnosc);
			Assert::AreEqual(1.0, domek_0.liczebnosc);

		}

		void SprawdzSilaGracza(int liczba, int produkcja, int kuznia, int szybkosc, tuple<int,int,int,int> wyliczone)
		{
			Assert::AreEqual(liczba, get<0>(wyliczone));
			Assert::AreEqual(produkcja, get<1>(wyliczone));
			Assert::AreEqual(kuznia, get<2>(wyliczone));
			Assert::AreEqual(szybkosc, get<3>(wyliczone));
		}

		TEST_METHOD(SilaGracza)
		{
			auto& domek_0 = tworca::DodajDomek(rozgrywka, 0, 10, PD());
			tworca::DodajDomek(rozgrywka, 1, 30, PD());
			tworca::DodajDomek(rozgrywka, 1, 7, PD());
			
			SprawdzSilaGracza(10, 1, 0, 0, rozgrywka.SilaGracza(0));
			SprawdzSilaGracza(37, 2, 0, 0, rozgrywka.SilaGracza(1));
			tworca::DodajLudka(rozgrywka, domek_0, 1, 12, PD());

			SprawdzSilaGracza(10, 1, 0, 0, rozgrywka.SilaGracza(0));
			SprawdzSilaGracza(49, 2, 0, 0, rozgrywka.SilaGracza(1));

			domek_0.typdomku = TypDomku::kZbrojownia;
			SprawdzSilaGracza(10, 0, 1, 0, rozgrywka.SilaGracza(0));

			domek_0.typdomku = TypDomku::kFort;
			SprawdzSilaGracza(10, 0, 0, 0, rozgrywka.SilaGracza(0));

			domek_0.typdomku = TypDomku::kStajnia;
			SprawdzSilaGracza(10, 0, 0, 1, rozgrywka.SilaGracza(0));
		}

		TEST_METHOD(Osy)
		{
			auto res = split_parent("placek\\nowy.txt");
			Assert::AreEqual("placek", res.first.c_str());
			Assert::AreEqual("nowy.txt", res.second.c_str());
		}
	};
}