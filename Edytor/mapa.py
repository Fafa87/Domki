from plansza import Plansza


class Mapa(Plansza):

    def __init__(self, h, w, nazwa_mapy, d=1):
        Plansza.__init__(self, h, w, d)
        self.nazwa_mapy = nazwa_mapy
        self.lista_domkow = []

    def __str__(self):
        return "{}: {}\n {}".format(self.nazwa_mapy, self.wielkosc_planszy_pola(), self.prezentacja())

    def statystyki(self):
        for i in
        return "{}\n".format(self.prezentacja)

    # Wypisać się w konsoli: plansza, drogi, statystyki
    # (#domków, #graczy, odległość między graczami).


    def wczytaj_txt(self, plik):
        slownik_domki = {}
        #TODO
        """
         jak wczytujemy z jakiegos pliku to ustawmy
         nazwa_mapy na nazwę tego pliku (bez rozszerzenia)
        """
        with open(plik, 'r') as mapa:
            for domek in range(int(mapa.readline())):
                nr_domku, l_info = mapa.readline().split(" ")
                lista_info = []
                for i in range(int(l_info)):
                    lista_info.append(mapa.readline().strip().split(" "))
                slownik_domki[nr_domku] = lista_info
        return slownik_domki

    def wczytaj_domek(self, para_nazwa_cechy_domka):
        """'1': [['koordynaty', '300', '400'], ['drogi', '1', '6'], ['gracz', '1']],"""
        for key in para_nazwa_cechy_domka:
            domek = Domek(key)
            for i in para_nazwa_cechy_domka[key]:
                if i[0] == 'koordynaty':
                    domek.x = int(i[1])
                    domek.y = int(i[2])
                if i[0] == 'drogi':
                    lista_drog = [j for j in i[1:]]
                    for j in lista_drog:
                        domek.dodaj_sasiada(int(j))
                if i[0] == 'gracz':
                    domek.gracz = int(i[1])
            self.lista_domkow.append(domek)
        return self.lista_domkow

    def usun_domek(self, nazwa):
        if nazwa in self.lista_domkow:
            self.lista_domkow.remove(nazwa)




class Domek:

    def __init__(self, numer, gracz=None):
        self.numer = numer
        self.nazwa = "domek" + str(numer)
        self.lista_sasiadow = []
        self.gracz = gracz

    def __repr__(self):
        return str(self.numer)

    def dodaj_sasiada(self, numer_domku):
        self.lista_sasiadow.append(numer_domku)





if __name__ == '__main__':
    mapa = Mapa(1, 1, "Pierwsza plansza")

    wynik = mapa.wczytaj_domek({'1': [['koordynaty', '300', '400'], ['drogi', '1', '6'], ['gracz', '1']],
                              '2': [['koordynaty', '1100', '400'], ['drogi', '1', '4'], ['gracz', '2']]})
    for i in wynik:
        print((i.x, i.y, i.lista_sasiadow, i.gracz))


"""Statyczna metoda Mapa.z_pliku(sciezka) tworząca obiekt.
Dodawać i usuwać drogi poprzez podanie ich początków i końców (drogi są dwukierunkowe):
dodaj_droge(h1,w1,h2,w2), usun_droge(h1,w1,h2,w2)
Wypisać się w konsoli: plansza, drogi, statystyki (#domków, #graczy, odległość między graczami).
Zwracać: listę graczy obecnych na mapie, listę istniejących dróg.
Zapisanie się do pliku jako mapa Domków, którą potem można uruchomić w grze.
Metoda obiektu mapa.zapisz(sciezka).
Sprawdzenie poprawności mapy (co najmniej dwóch graczy, między dowolnymi dwoma domkami da się przejść)."""

