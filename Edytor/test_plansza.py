# -*- coding: ISO-8859-2 -*-
import unittest
from plansza import Plansza


class TestPlansza(unittest.TestCase):
    def test_nowa_plansza(self):
        nowa_plansza = Plansza(1, 1)
        self.assertEqual(1, nowa_plansza.d)

    def test_wielkosc_planszy(self):
        nowa_plansza = Plansza(1, 1, 10)
        self.assertEqual((10, 10), nowa_plansza.wielkosc_planszy())
        nowa_plansza = Plansza(2, 3, 10)
        self.assertEqual((20, 30), nowa_plansza.wielkosc_planszy())

    def test_punkt_na_pole(self):
        nowa_plansza = Plansza(10, 10, 10)
        self.assertEqual((0, 0), nowa_plansza.punkt_na_pole(1, 9))
        self.assertEqual((10, 10), nowa_plansza.punkt_na_pole(100, 100))
        self.assertEqual((1, 2), nowa_plansza.punkt_na_pole(15, 25))
        self.assertEqual((1, 2), nowa_plansza.punkt_na_pole(18, 23))
        nowa_plansza = Plansza(10, 10, 1)
        self.assertEqual((0, 0), nowa_plansza.punkt_na_pole(0, 0))
        self.assertEqual((1, 0), nowa_plansza.punkt_na_pole(1, 0))


    def test_pole_na_punkt(self):
        nowa_plansza = Plansza(10, 10, 10)
        self.assertEqual((5, 5), nowa_plansza.pole_na_punkt(0, 0))
        nowa_plansza = Plansza(10, 10)
        self.assertEqual((1.5, 1.5), nowa_plansza.pole_na_punkt(1, 1))

    def test_wstaw(self):
        nowa_plansza = Plansza(3, 3)
        with self.assertRaises(IndexError) as error:
            nowa_plansza.wstaw(-1, 11, "obiekt")
        self.assertEqual("list index out of range", str(error.exception))
        nowa_plansza.wstaw(1, 1, "obiekt")
        self.assertFalse(nowa_plansza.wstaw(1, 1, "obiekt"))
        nowa_plansza = Plansza(3, 3)
        self.assertTrue(nowa_plansza.wstaw(0, 0, "obiekt"))

    def test_wyglad_planszy(self):
        nowa_plansza = Plansza(3, 3)
        self.assertEqual("[['.', '.', '.'], ['.', '.', '.'], ['.', '.', '.']]", str(nowa_plansza))

    def test_zapytanie_punkt(self):
        nowa_plansza = Plansza(20, 20)
        self.assertEqual(None, nowa_plansza.zapytanie_punkt(5, 5))
        nowa_plansza.wstaw(4, 4, "obiekt")
        self.assertEqual((4.5, 4.5), nowa_plansza.pole_na_punkt(4,4))
        self.assertEqual("obiekt", nowa_plansza.zapytanie_punkt(4, 4))

    def test_zapytanie_pole(self):
        nowa_plansza = Plansza(20, 20)
        self.assertEqual(None, nowa_plansza.zapytanie_pole(5, 5))
        nowa_plansza.wstaw(4, 4, "obiekt")
        self.assertEqual("obiekt", nowa_plansza.zapytanie_pole(4, 4))

    def test_czyszczenie_pola(self):
        nowa_plansza = Plansza(10, 10)
        nowa_plansza.wstaw(4, 4, "obiekt")
        self.assertEqual('obiekt', nowa_plansza.zapytanie_pole(4, 4))
        nowa_plansza.wstaw(3, 4, "obiekt")
        nowa_plansza.czyszczenie_pola(4, 4)
        self.assertEqual(None, nowa_plansza.zapytanie_pole(4, 4))
        self.assertEqual('obiekt', nowa_plansza.zapytanie_pole(3, 4))

    def test_czyszczenie_przez_punkt(self):
        nowa_plansza = Plansza(10, 10)
        nowa_plansza.wstaw(4, 4, "obiekt")
        self.assertEqual('obiekt', nowa_plansza.zapytanie_pole(4, 4))
        nowa_plansza.wstaw(3, 4, "obiekt")
        nowa_plansza.czyszczenie_przez_punkt(4, 4)
        self.assertEqual(None, nowa_plansza.zapytanie_pole(4, 4))
        self.assertEqual('obiekt', nowa_plansza.zapytanie_pole(3, 4))

    def test_wielkosc_planszy_pola(self):
        nowa_plansza = Plansza(10,10)
        nowa_plansza1 = Plansza(0,1)
        nowa_plansza2 = Plansza(0,0)
        self.assertEqual((10,10), nowa_plansza.wielkosc_planszy_pola())
        self.assertEqual((0,1), nowa_plansza1.wielkosc_planszy_pola())
        self.assertEqual((0,0), nowa_plansza2.wielkosc_planszy_pola())

    def test_wielkosc_planszy_punkty(self):
        nowa_plansza = Plansza(10,10,10)
        nowa_plansza1 = Plansza(0,1,10)
        nowa_plansza2 = Plansza(0,0,100)
        self.assertEqual((100,100), nowa_plansza.wielkosc_planszy_punkty())
        self.assertEqual((0,10), nowa_plansza1.wielkosc_planszy_punkty())
        self.assertEqual((0,0), nowa_plansza2.wielkosc_planszy_punkty())

    def test_podaj_obiekty(self):
        nowa_plansza = Plansza(2,2)
        self.assertEqual({}, nowa_plansza.podaj_obiekty())
        nowa_plansza.wstaw(1, 1, "dab")
        self.assertEqual({(1,1): "dab"}, nowa_plansza.podaj_obiekty())
        nowa_plansza.wstaw(1, 0, "abra")
        self.assertEqual({(1,0): "abra", (1,1): "dab"}, nowa_plansza.podaj_obiekty())


if __name__ == '__main__':
    unittest.main()
