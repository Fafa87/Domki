from plansza import Plansza
import os


class Mapa(Plansza):
    def __init__(self, h, w, nazwa_mapy, d=1):
        Plansza.__init__(self, h, w, d)
        self.nazwa_mapy = nazwa_mapy
        self.lista_domkow = []

    def __str__(self):
        return "{}: {}\n {}".format(self.nazwa_mapy, self.wielkosc_planszy_pola(), self.prezentacja())

    def wczytaj_txt(self, plik):
        slownik_domki = {}
        self.nazwa_mapy = os.path.splitext(os.path.basename(plik))[0]
        with open(plik, 'r') as mapa:
            for domek in range(int(mapa.readline())):
                nr_domku, l_info = mapa.readline().split(" ")
                lista_info = []
                for i in range(int(l_info)):
                    lista_info.append(mapa.readline().strip().split(" "))
                slownik_domki[nr_domku] = lista_info
        return slownik_domki

    def wczytaj_domki(self, para_nazwa_cechy_domka):
        """'1': [['koordynaty', '300', '400'], ['drogi', '1', '6'], ['gracz', '1']],"""
        for key in para_nazwa_cechy_domka:
            domek = Domek(key)
            for i in para_nazwa_cechy_domka[key]:
                if i[0] == 'koordynaty':
                    domek.y = int(i[1])
                    domek.x = int(i[2])
                if i[0] == 'drogi':
                    drogi = i[2:]
                    for j in drogi:
                        domek.dodaj_sasiada(int(j))
                if i[0] == 'gracz':
                    domek.gracz = int(i[1])
            self.lista_domkow.append(domek)
        return self.lista_domkow

    def domek_na_plansze(self):
        for domek in self.lista_domkow:
            h, w = self.punkt_na_pole(domek.y, domek.x)
            self.wstaw(h, w, domek)

    def usun_domek(self, nazwa):
        if nazwa in self.lista_domkow:
            self.lista_domkow.remove(nazwa)

    def drogi_do_domku(self, domek):
        lista_drog = []
        lista_drog_krotka = []
        for sasiad in domek.lista_sasiadow:
            lista_drog.append([int(domek.numer), int(sasiad)])
        for listy in lista_drog:
            listy.sort()
        for listy in lista_drog:
            lista = tuple(listy)
            lista_drog_krotka.append(lista)
        lista_drog_krotka = list(set(lista_drog_krotka))
        return lista_drog_krotka

    def statystyki(self):
        slownik = {}
        lista_graczy = []
        for domek in self.lista_domkow:
            if domek.gracz:
                lista_graczy.append(domek.gracz)
        lista_drog = []
        for domek in self.lista_domkow:
            for droga in self.drogi_do_domku(domek):
                lista_drog.append(droga)
        lista_drog = list(set(lista_drog))
        slownik['gracze'] = len(lista_graczy)
        slownik['liczba_drog'] = len(lista_drog)
        slownik['lista_drog'] = lista_drog
        slownik['liczba_domkow'] = len(self.lista_domkow)
        return slownik

    def pokaz_statystyki(self, slownik):
        return "Liczba graczy: {}\nLiczba dróg: {}\nDrogi:{}\nLiczba domków: {}\n".format(slownik['gracze'],
                                                                                          slownik['liczba_drog'],
                                                                                          slownik['lista_drog'],                                                                              slownik['liczba_domkow'])

    def dodaj_droge(self, h1, w1, h2, w2):
        if isinstance(self.zapytanie_pole(h1, w1), Domek) and isinstance(self.zapytanie_pole(h2, w2), Domek):
            self.zapytanie_pole(h1, w1).dodaj_sasiada(self.zapytanie_pole(h2, w2).numer)
            self.zapytanie_pole(h2, w2).dodaj_sasiada(self.zapytanie_pole(h1, w1).numer)
            return True
        return False


class Domek(object):
    def __init__(self, numer, gracz=None):
        self.numer = numer
        self.nazwa = "domek" + str(numer)
        self.lista_sasiadow = []
        self.gracz = gracz

    def __repr__(self):
        return str(self.numer)

    def dodaj_sasiada(self, numer_domku):
        self.lista_sasiadow.append(numer_domku)


"""inty w domkach"""

if __name__ == '__main__':
    mapa = Mapa(15, 15, "Pierwsza plansza", 100)
    wynik = mapa.wczytaj_domki({'1': [['koordynaty', '300', '400'], ['drogi', '1', '2'], ['gracz', '1']],
                                '2': [['koordynaty', '1100', '400'], ['drogi', '1', '1'], ['gracz', '2']],
                                '3': [['koordynaty', '100', '400'], ['drogi', '2', '1', '4']]})
    mapa.domek_na_plansze()
    for i in wynik:
        print((i.y, i.x, i.lista_sasiadow, i.gracz))
    print(mapa.prezentacja())
    mapa.dodaj_droge(3, 4, 1, 4)
    dom1 = Domek(1, 1)
    dom1.dodaj_sasiada(2)
    dom1.dodaj_sasiada(3)
    print(mapa.drogi_do_domku(dom1))
    print(mapa.pokaz_statystyki(mapa.statystyki()))

"""
Wypisać się w konsoli: plansza, drogi, statystyki (#domków, #graczy, odległość między graczami).
Zwracać: listę graczy obecnych na mapie, listę istniejących dróg.
Zapisanie się do pliku jako mapa Domków, którą potem można uruchomić w grze.
Metoda obiektu mapa.zapisz(sciezka).
Sprawdzenie poprawności mapy (co najmniej dwóch graczy, między dowolnymi dwoma domkami da się przejść)."""
