#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/gui.h"

#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "../MultiSerwer/serwery.h"
#include "../MultiSerwer/mastery.h"

#include "easylogging++.h" 
INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace multi;
bool cichociemny = false;

void wykonaj(string zadanie);

void wybierz_i_wystartuj_tryb(string tryb, string komenda)
{
    char tmp[1000];
    if (!tryb.size())
    {
        printf("Zdecyduj kim jestes:\n"
            "- serwer - prowadzisz gre dla wielu graczy\n"
            "- klient - chcesz dolaczyc do gry zalozonej przez serwer\n"
            "- masterserwer - prowadzisz pokoje dla graczy z calego swiata i stawiasz serwery\n"
            "- masterklient - poszukujesz graczy aby zagrac przez internet\n");
        
        gets_s(tmp);
        tryb = tmp;
    }

    if (tryb == "serwer" || tryb == "s")
    {
        if (!komenda.size())
        {
            printf("Jaka plansza (folder nazwa)? Opcjonalnie do ilu wygranych.\nA dokladnie mozna podac: string folder, string mapa, int do_ilu, double szybkosc, int ile_ludzi\n");
            gets_s(tmp);
            komenda = tmp;
        }
        start_serwer_gry(komenda);
    }
    else if (tryb == "klient" || tryb == "k")
    {
        if (!komenda.size())
        {
            printf("Jak sie nazywasz?\n");
        }
        start_klient_gry(komenda);
    }
    else if (tryb == "masterserwer" || tryb == "ms")
    {
        if (!komenda.size())
        {
            printf("Podaj port:\n");
            gets_s(tmp);
            komenda = tmp;
        }
        start_masterserwer(stoi(komenda));
    }
    else if (tryb == "masterklient" || tryb == "mk")
    {
        if (!komenda.size())
        {
            printf("Jak sie nazywasz?\n");
            gets_s(tmp);
            komenda = tmp;
        }
        start_masterklient(komenda);
    }
}

void komunikat()
{
    auto& serwer = KontekstGry::o().serwer;
    auto& klient = KontekstGry::o().klient;
    auto& masterserwer = KontekstSwiata::o().serwer;
    auto& masterklient = KontekstSwiata::o().klient;

    if (klient != nullptr)
        komunikat_serwer_klient();
    else if (serwer != nullptr)
        komunikat_serwer_gry();
    else if (masterserwer != nullptr)
        komunikat_masterserwer(masterserwer);
    else if (masterklient != nullptr)
        komunikat_masterklient(masterklient);
}

void wykonaj(string zadanie)
{
    if (zadanie.size() > 0)
        LOG(INFO) << "Wykonuje: '" << zadanie << "'";

    auto& serwer = KontekstGry::o().serwer;
    auto& klient = KontekstGry::o().klient;
    auto& masterserwer = KontekstSwiata::o().serwer;
    auto& masterklient = KontekstSwiata::o().klient;

    if (klient != nullptr)
        wykonaj_klient_gry(zadanie);
    else if (serwer != nullptr)
        wykonaj_serwer_gry(zadanie);
    else if (masterserwer != nullptr)
        wykonaj_masterserwer(masterserwer, zadanie);
    else if (masterklient != nullptr)
        wykonaj_masterklient(masterklient, zadanie);
}

void konfiguruj(int l, const char * argv[])
{
    auto& misja_ustawienia = KontekstGry::o().misja_ustawienia;
    auto& serwer = KontekstGry::o().serwer;
    auto& klient = KontekstGry::o().klient;

    if (l == 0)
    {
        LOG(INFO) << "Konfiguruje serwer...";
        wybierz_i_wystartuj_tryb("serwer", string(argv[2]) + " " + string(argv[3]) + " " + string(argv[4]) + " " + string(argv[5]) + " " + string(argv[6]));
        while (misja_ustawienia.Zwyciezca() < 0)
            wykonaj("start");
        exit(0);
    }
    else if (l == 1)
    {
        LOG(INFO) << "Konfiguruje klienta...";
        wybierz_i_wystartuj_tryb("klient", std::to_string(l));
        pobierz_serwery_gry();
    }
}

int main(int argc, const char * argv[]) {
    string zadanie;

    el::Configurations conf("MultiSerwer_log.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    LOG(INFO) << "MultiSerwer odpalony!";
    
    GUI::aplikacja().setup_theme();
    GUI::aplikacja().okno.close();
    if (argc > 1)
    {
        cichociemny = true;
        konfiguruj(int(argv[1][0] - '0'), argv);
    }

    wybierz_i_wystartuj_tryb("", "");

    auto serwer = KontekstGry::o().serwer;
    auto klient = KontekstGry::o().klient;

    try {
        do
        {
            komunikat();
            char tmp[1000];
            gets_s(tmp);
            zadanie = tmp;

            wykonaj(zadanie);

            if (klient != nullptr)
                pobierz_serwery_gry();

            Sleep(10);
        } while (zadanie != "k");
    }
    catch (const std::exception& ex)
    {
        LOG(ERROR) << "Wyjatek ladny: " << ex.what();
        el::Loggers::flushAll();
        system("pause");
        return -1;
    }
    catch (const std::string& ex) {
        LOG(ERROR) << "Wyjatek string: " << ex;
        el::Loggers::flushAll();
        system("pause");
        return -1;
    }
    catch (int err) {
        LOG(ERROR) << "Wyjatek err: " << err;
        el::Loggers::flushAll();
        system("pause");
        return -1;
    }
    catch (...) {
        
        LOG(ERROR) << "Wyjatek noname";
        el::Loggers::flushAll();
        system("pause");
        return -1;
    }
    LOG(INFO) << "MultiSerwer zamkniety";
    system("pause");
    return 0;
}
