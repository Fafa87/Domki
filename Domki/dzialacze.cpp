#include "dzialacze.h"

MyszDecydent::MyszDecydent(Rozgrywka & rozgrywka, Gracz & gracz) : rozgrywka(rozgrywka), gracz(gracz)
{
}

void MyszDecydent::Klik(double x, double y)
{
}

void MyszDecydent::Przetworz(sf::Event zdarzenie)
{
}

void MyszDecydent::WykonajRuch()
{
}



Ruszacz::Ruszacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
}

void Ruszacz::Ruszaj()
{
}

void Ruszacz::PrzesuwajLudkow()
{
}

void Ruszacz::WalczLudkami()
{
}

void Ruszacz::Produkuj()
{
}

Wyswietlacz::Wyswietlacz(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
}

void Wyswietlacz::Wyswietlaj(sf::RenderWindow & okno)
{
	// powinien robi� draw na wcze�niej wygenerowanych obiektach typu ShapeCircle
	// powinien sprawdzi� czy jeszcze s� w grze, lub jakie obiekty dosz�y i zapewni� aby ka�dy obiekt
	// mia� sw�j kszta�t
	//okno.draw(shape);
}
