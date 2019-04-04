#include "muzyka.h"

Muzykant::Muzykant(Rozgrywka & rozgrywka) : rozgrywka(rozgrywka)
{
}

void Muzykant::Zaladuj(string wybrana_skora)
{
	przygrywka.openFromFile("Muzyka\\" + wybrana_skora + "\\Dawnosc.ogg");
	przygrywkaOkrzyki.openFromFile("Muzyka\\" + wybrana_skora + "\\Okrzykowo.ogg");
	marszBuffer.loadFromFile("Muzyka\\" + wybrana_skora + "\\Marsz.ogg");
	marsz.setBuffer(marszBuffer);
	zdobycieBuffer.loadFromFile("Muzyka\\" + wybrana_skora + "\\Tupniecie.ogg");
	zdobycie.setBuffer(zdobycieBuffer);
}

void Muzykant::NoGraj()
{
	if (przygrywka.getStatus() == sf::SoundSource::Status::Stopped && 
		przygrywkaOkrzyki.getStatus() == sf::SoundSource::Status::Stopped)
	{
		przygrywkaOkrzyki.setVolume(70);
		przygrywkaOkrzyki.play();
	}
}

void Muzykant::Przygrywaj()
{
	przygrywka.setVolume(20);
	przygrywka.play();
}

void Muzykant::Zamilcz()
{
	przygrywka.stop();
	przygrywkaOkrzyki.stop();
}

void Muzykant::GrajEfekty(Ruszacz & ruchy)
{
	if (ruchy.ile_armii_idzie > 0)
	{
		marsz.setVolume(min(100, 20 * ruchy.ile_armii_idzie));
		if (marsz.getStatus() == sf::Music::Status::Stopped)
		{
			marsz.play();
		}
	}

	if (ruchy.armie_ktore_dotarly > 0)
	{
		zdobycie.play();
	}
}
