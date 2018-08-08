import unittest
from plansza import Plansza
from mapa import Mapa
from mapa import Domek


class TestMapa(unittest.TestCase):

    def test_wczytaj_txt(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)
        self.assertEqual({'1': [['koordynaty', '300', '400'], ['drogi', '1', '6'], ['gracz', '1']],
                          '2': [['koordynaty', '1100', '400'], ['drogi', '4', '4', '6', '9', '8'], ['gracz', '2']]},
                         mapka.wczytaj_txt('mapa_testowa.txt'))

    def test_wczytaj_domek(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)
        slowniczek = mapka.wczytaj_txt('mapa_testowa.txt')
        lista = mapka.wczytaj_domki(slowniczek)
        self.assertEqual((400, 1100 , [6], [4, 6, 9, 8]), (lista[1].x, lista[1].y, lista[0].lista_sasiadow, lista[1].lista_sasiadow))

    def test_statystyki(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)

        """for i in
        return "{}\n".format(self.prezentacja)"""

    # Wypisać się w konsoli: plansza, drogi, statystyki
    # (#domków, #graczy, odległość między graczami).
