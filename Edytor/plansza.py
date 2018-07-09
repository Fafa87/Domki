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

    def wstawianie_obiektu(self,h,w,obiekt):
        if self.siatka[h][w] == '.':
            self.siatka[h][w] = obiekt
            return True
        elif self.siatka[h][w] != '.':
            return False




plansza = Plansza(3,3)
print(plansza)













"""
Zapytanie o zawartość pola (H, W) lub przez punkt (Y, X).
Możliwość wyczyszczenia pola (H, W) lub przez punkt (Y, X).
Aktualny rozmiar planszy w polach i w rozmiarze.
Obrót całej planszy, odbicia lustrzane poziome i pionowe.
Prezentacja zawartości planszy w konsoli.
Podaj obiekty i ich położenia"""
