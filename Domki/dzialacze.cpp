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
	// powinien robiæ draw na wczeœniej wygenerowanych obiektach typu ShapeCircle
	// powinien sprawdziæ czy jeszcze s¹ w grze, lub jakie obiekty dosz³y i zapewniæ aby ka¿dy obiekt
	// mia³ swój kszta³t
	//okno.draw(shape);
}
