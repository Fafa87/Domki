#include "CppUnitTest.h"

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

		TEST_METHOD(SilaGracza)
		{
			auto& domek_0 = tworca::DodajDomek(rozgrywka, 0, 10, PD());
			tworca::DodajDomek(rozgrywka, 1, 30, PD());
			tworca::DodajDomek(rozgrywka, 1, 7, PD());

			Assert::AreEqual(10, rozgrywka.SilaGracza(0));
			Assert::AreEqual(37, rozgrywka.SilaGracza(1));

			tworca::DodajLudka(rozgrywka, domek_0, 1, 12, PD());

			Assert::AreEqual(10, rozgrywka.SilaGracza(0));
			Assert::AreEqual(49, rozgrywka.SilaGracza(1));
		}

		TEST_METHOD(Osy)
		{
			auto res = split_parent("placek\\nowy.txt");
			Assert::AreEqual("placek", res.first.c_str());
			Assert::AreEqual("nowy.txt", res.second.c_str());
		}

	};
}