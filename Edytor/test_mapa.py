import unittest
from plansza import Plansza
from mapa import Mapa
from mapa import Domek


class TestMapa(unittest.TestCase):

    def test_wczytaj_txt(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)
        self.assertEqual({'1': [['koordynaty', '300', '400'], ['drogi', '1', '6'], ['gracz', '1']],
                          '2': [['koordynaty', '1100', '400'], ['drogi', '1', '4'], ['gracz', '2']]},
                         mapka.wczytaj_txt('mapa_testowa.txt'))

    def test_wczytaj_domek(self):
        mapka = Mapa(10, 10, "nowa_mapa", 100)
        slownik = mapka.wczytaj_txt('mapa_testowa.txt')
        domek1 = Domek(1)
        domek1.x = 300
        domek1.y = 400
        domek1.lista_sasiadow = [1, 6]
        domek1.gracz = 1
        domek2 = Domek(2)
        domek2.x = 1100
        domek2.y = 400
        domek2.lista_sasiadow = [1, 4]
        domek2.gracz = 2
        self.assertEqual([domek1, domek2], list(mapka.wczytaj_domek(slownik)))
