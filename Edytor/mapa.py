from plansza import Plansza


class Mapa(Plansza):

    def __init__(self, h, w, nazwa_mapy, d=1):
        Plansza.__init__(self, h, w, d)
        self.nazwa_mapy = nazwa_mapy

    def __str__(self):
        return "{}: {}\n {}".format(self.nazwa_mapy, self.wielkosc_planszy_pola(), self.prezentacja())


class Domek:

    def __init__(self, y, x, numer_gracza=1):
        self.numer_gracza = numer_gracza
        self.x = x
        self.y = y
        self.lista_sasiadow = []

    def __repr__(self):
        return str(self.numer_gracza)

    def dodaj_sasiada(self, x, y, numer_gracza):
        nowy_sasiad = Domek(x, y, numer_gracza)
        self.lista_sasiadow.append(nowy_sasiad)

mapa = Mapa(1, 1, "Pierwsza plansza")
print(mapa)


"""Utworzenie nowej mapy o zadanych wymiarach.
Wczytać wszystkie istotne informacje z pliku mapy Domków.
Statyczna metoda Mapa.z_pliku(sciezka) tworząca obiekt.
Dodawać i usuwać drogi poprzez podanie ich początków i końców (drogi są dwukierunkowe):
dodaj_droge(h1,w1,h2,w2), usun_droge(h1,w1,h2,w2)
Wypisać się w konsoli: plansza, drogi, statystyki (#domków, #graczy, odległość między graczami).
Zwracać: listę graczy obecnych na mapie, listę istniejących dróg.
Zapisanie się do pliku jako mapa Domków, którą potem można uruchomić w grze.
Metoda obiektu mapa.zapisz(sciezka).
Sprawdzenie poprawności mapy (co najmniej dwóch graczy, między dowolnymi dwoma domkami da się przejść)."""

