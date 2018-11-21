#include "CppUnitTest.h"

#include "../Domki/rozgrywka.h"
#include "../Domki/os.h"

#include "tworca.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DomkoTesty
{
	TEST_CLASS(DzialaczeTest)
	{
		Rozgrywka rozgrywka;
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			rozgrywka = Rozgrywka();
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