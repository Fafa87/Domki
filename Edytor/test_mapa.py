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

    def test_pokaz_statystyki(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)
        slowniczek = mapka.wczytaj_txt('mapa_testowa.txt')
        lista = mapka.wczytaj_domki(slowniczek)
        self.assertEqual('Liczba graczy: 2\nLiczba dróg: 5\nDrogi:[(2, 6), (2, 9), (2, 8), (1, 6), (2, 4)]\nLiczba domków: 2\n', mapka.pokaz_statystyki(mapka.statystyki()))


    # Wypisać się w konsoli: plansza, , statystyki
    # (#domków, #graczy, odległość między graczami).
