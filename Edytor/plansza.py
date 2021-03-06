# -*- coding: ISO-8859-2 -*-
from copy import deepcopy


class Plansza:

    def __init__(self, h, w, d=1):
        self.h = h
        self.w = w
        self.d = d #dokładność
        self.siatka = []
        a = self.w * ['.']
        for i in range(self.h):
            self.siatka.append(list(a))

    def __repr__(self):
        return str(self.siatka)

    def __str__(self):
        return self.prezentacja()

    def punkt_na_pole(self, y, x):
        h = int(y/self.d)
        w = int(x/self.d)
        return h, w

    def pole_na_punkt(self, h, w):
        y = (h*self.d + 0.5*self.d)
        x = (w*self.d + 0.5*self.d)
        return y, x

    def wstaw(self, h, w, obiekt):
        if self.siatka[h][w] == '.':
            self.siatka[h][w] = obiekt
            return True
        elif self.siatka[h][w] != '.':
            return False

    def zapytanie_punkt(self, y, x):
        h, w = self.punkt_na_pole(y, x)
        return self.zapytanie_pole(h, w)

    def zapytanie_pole(self, h, w):
        if self.siatka[h][w] == '.':
            return None
        else:
            return self.siatka[h][w]

    def czyszczenie_pola(self, h, w):
        if self.siatka[h][w] == '.':
            return None
        else:
            self.siatka[h][w] = '.'

    def czyszczenie_przez_punkt(self, y, x):
        h, w = self.punkt_na_pole(y, x)
        return self.czyszczenie_pola(h, w)

    def wielkosc_planszy_pola(self):
        return self.h, self.w

    def wielkosc_planszy_punkty(self):
        y = self.h * self.d
        x = self.w * self.d
        return y, x

    def siatka_prezentacja(self):
        siata = deepcopy(self.siatka)
        for i in range(self.h):
            for j in range(self.w):
                if len(siata[i][j])>1:
                    siata[i][j] = siata[i][j][0]
        return siata

    def prezentacja(self):
        siatka = self.siatka_prezentacja()
        lista = []
        for i in range(self.h):
            x = (" ".join(siatka[i]))
            lista.append(x)
        return "\n".join(lista)

    def podaj_obiekty(self):
        slownik = {}
        for i in range(len(self.siatka)):
            for j in range(len(self.siatka[i])):
                if self.siatka[i][j] != ".":
                    slownik[(i, j)] = self.siatka[i][j]
                else:
                    continue
        return slownik

    def lustro_poz(self):
        nowa_siatka = []
        for i in range(len(self.siatka)):
            nowa_siatka.append(self.siatka[i][::-1])
        self.siatka = nowa_siatka
        return self.siatka

    def lustro_pion(self):
        self.siatka = self.siatka[::-1]
        return self.siatka

    def obrot_90(self):
        self.siatka = list(zip(*self.siatka[::-1]))
        self.siatka = list(map(list, self.siatka))
        return self.siatka

    def obrot_stopnie(self, stopnie):
        if stopnie%90 == 0:
            razy_obrot = int(stopnie/90)
            for i in range(razy_obrot):
                self.obrot_90()
            return self.siatka
        else:
            print("Stopnie jako wielokrotność liczby 90")








if __name__ == '__main__':
    """plansza = Plansza(4, 4)
    plansza.wstawianie_obiektu(0, 0, 'aj')
    plansza.wstawianie_obiektu(0, 1, 'ba')
    plansza.wstawianie_obiektu(1, 0, 'ca')
    plansza.prezentacja()
    print("________________")
    plansza.lustro_poz()
    plansza.prezentacja()
    print("________________")
    plansza = Plansza(4, 4)
    plansza.wstawianie_obiektu(0, 0, 'aj')
    plansza.wstawianie_obiektu(0, 1, 'ba')
    plansza.wstawianie_obiektu(1, 0, 'ca')
    plansza.lustro_pion()
    plansza.prezentacja()
    print("________________")"""
    plansza = Plansza(4, 4)
    plansza.wstaw(0, 0, 'aj')
    plansza.wstaw(0, 1, 'ba')
    plansza.wstaw(1, 0, 'ca')
    plansza.prezentacja()
    print(plansza.prezentacja())
    print(plansza)



