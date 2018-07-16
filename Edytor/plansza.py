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

    def wielkosc_planszy(self):
        px_h = self.h*self.d
        px_w = self.w*self.d
        return px_h, px_w

    def punkt_na_pole(self, y, x):
        h = int(y/self.d)
        w = int(x/self.d)
        return h, w

    def pole_na_punkt(self, h, w):
        y = (h*self.d + 0.5*self.d)
        x = (w*self.d + 0.5*self.d)
        return y, x

    def wstawianie_obiektu(self,h,w,obiekt):
        if self.siatka[h][w] == '.':
            self.siatka[h][w] = obiekt
            return True
        elif self.siatka[h][w] != '.':
            return False

    def zapytanie_punkt(self, y, x):
        h, w = self.punkt_na_pole(y, x)
        if self.siatka[h][w] == '.':
            return "Pole wolne"
        else:
            return "Na polu stoi {}".format(self.siatka[h][w])

    def zapytanie_pole(self, h, w):
        if self.siatka[h][w] == '.':
            return "Pole wolne"
        else:
            return "Na polu stoi {}".format(self.siatka[h][w])

    def czyszczenie_pola(self, h, w):
        if self.siatka[h][w] == '.':
            return "Pole wolne"
        else:
            self.siatka[h][w] = '.'

    def czyszczenie_przez_punkt(self, y, x):
        h, w = self.punkt_na_pole(y, x)
        if self.siatka[h][w] == '.':
            return "Pole wolne"
        else:
            self.siatka[h][w] = '.'

    def ile_pol(self):
        return self.h, self.w

    def ile_punktow(self):
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
            x = ("       ".join(siatka[i]))
            lista.append(x)
        for i in range(self.h):
            print(lista[i])





if __name__ == '__main__':
    plansza = Plansza(5, 5)
    plansza.wstawianie_obiektu(0, 0, 'aj')
    plansza.wstawianie_obiektu(0, 1, 'ba')
    plansza.prezentacja()











"""
Obrót całej planszy, odbicia lustrzane poziome i pionowe.
Podaj obiekty i ich położenia"""
