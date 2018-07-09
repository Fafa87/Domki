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
        self.assertEqual((0, 0), nowa_plansza.punkt_na_pole(5, 5))
        nowa_plansza = Plansza(10, 10, 10)
        self.assertEqual((1, 2), nowa_plansza.punkt_na_pole(15, 25))
        nowa_plansza = Plansza(10, 10, 10)
        self.assertEqual((1, 2), nowa_plansza.punkt_na_pole(18, 23))

    def test_pole_na_punkt(self):
        nowa_plansza = Plansza(10, 10, 10)
        self.assertEqual((5, 5), nowa_plansza.pole_na_punkt(0, 0))

    def test_wstawianie_obiektu(self):
        nowa_plansza = Plansza(3, 3)
        """self.assertRaises(("Nie ma takiego pola"), nowa_plansza.wstawianie_obiektu(-1, 11, "obiekt"))"""
        nowa_plansza.wstawianie_obiektu(1, 1, "obiekt")
        self.assertEqual(False, nowa_plansza.wstawianie_obiektu(1, 1, "obiekt"))
        nowa_plansza = Plansza(3, 3)
        self.assertEqual(True, nowa_plansza.wstawianie_obiektu(0, 0, "obiekt"))

    def test_wyglad_planszy(self):
        nowa_plansza = Plansza(3, 3)
        self.assertEqual("[['.', '.', '.'], ['.', '.', '.'], ['.', '.', '.']]", str(nowa_plansza))


"""Wstawianie obiektu (tekstu) do podanego pola (H, W) (rzuca wyjątek gdy H < 0 lub W < 0 lub gdy pole zajęte)."""

if __name__ == '__main__':
    unittest.main()
