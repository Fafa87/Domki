class Plansza:

    def __init__(self, h, w, d=1):
        self.h = h
        self.w = w
        self.d = d #dokładność

    def wielkosc_planszy(self):
        px_h = self.h*self.d
        px_w = self.w*self.d
        return px_h, px_w

    def punkt_na_pole(self, y, x):
        h = int(y/self.d)
        w = int(x/self.d)
        return h, w

    def pole_na_punkt(self, h, w):
        y = h*self.d + 0.5*self.d
        x = w*self.d + 0.5*self.d
        return y, x







"""Wstawianie obiektu (tekstu) do podanego pola (H, W) (rzuca wyjątek gdy H < 0 lub W < 0 lub gdy pole zajęte).
Zapytanie o zawartość pola (H, W) lub przez punkt (Y, X).
Możliwość wyczyszczenia pola (H, W) lub przez punkt (Y, X).
Aktualny rozmiar planszy w polach i w rozmiarze.
Obrót całej planszy, odbicia lustrzane poziome i pionowe.
Prezentacja zawartości planszy w konsoli.
Podaj obiekty i ich położenia"""
