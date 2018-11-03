#include "CppUnitTest.h"

#include "../Domki/misja.h"
#include "../Domki/dane.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DomkoTesty
{		
	TEST_CLASS(DzialaczeTest)
	{
		Rozgrywka rozgrywka;
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
			rozgrywka = zwarcie_rozgrywka("mini_mapa_test.txt");
		}

		TEST_METHOD(WalkaZamku)
		{
		}

	};
}