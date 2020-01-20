#include "../MultiSerwer/mastery.h"

#include <thread>

KontekstSwiata* KontekstSwiata::obiekt;

void komunikat_masterserwer(mastery::Serwer* serwer)
{
    if (serwer->dziala)
        printf("Jestes postawionym multiserwerem. Napisz stop aby umrzec:'\n");
    else
        printf("Jestes spiacym multiserwerem. Napisz postaw <port> aby sie obudzic.'\n");
}

void start_masterserwer(int port)
{
    KontekstSwiata::o().serwer = new mastery::Serwer();
    auto& serwer = KontekstSwiata::o().serwer;
    std::thread([&serwer, port]() { serwer->Postaw(port); }).detach();
}

void wykonaj_masterserwer(mastery::Serwer* serwer, string zadanie)
{
    if (zadanie.find("stop") == 0 && serwer->dziala == true)
    {
        LOG(INFO) << "Zatrzymuje serwer...";
        serwer->dziala = false;
    }
    if (zadanie.find("postaw") == 0 && serwer->dziala == false)
    {
        auto port = stoi(zadanie.substr(7));
        std::thread([serwer, port]() { serwer->Postaw(port); }).detach();
    }
}

void mastery::Serwer::Postaw(int port)
{
    LOG(INFO) << "Serwer stoi na porcie: " << port;
    this->dziala = true;
    while (this->dziala)
    {
        // sprawdz czy nikt nie chce sie dostac

        // sprawdz czy nikt nic nie pisze

        Sleep(100);
    }
    LOG(INFO) << "Serwer wylaczony.";
    this->dziala = false;
}