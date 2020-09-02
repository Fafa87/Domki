#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/gui.h"

#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "../MultiSerwer/serwery.h"

#include "easylogging++.h" 
INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace multi;
bool cichociemny = false;

void wykonaj(string zadanie);
void konfiguruj(int l, const char * argv[])
{
    auto& misja_ustawienia = Kontekst::o().misja_ustawienia;
    auto& serwer = Kontekst::o().serwer;
    auto& klient = Kontekst::o().klient;

    if (l == 0)
    {
        LOG(INFO) << "Konfiguruje serwer...";

        wykonaj("serwer " + string(argv[2]) + " " + string(argv[3]) + " " + string(argv[4]) + " " + string(argv[5]) + " " + string(argv[6]));
        while(misja_ustawienia.Zwyciezca() < 0)
            wykonaj("start");
        exit(0);
    }
    else if (l==1)
    { 
        LOG(INFO) << "Konfiguruje klienta...";

        wykonaj("klient " + std::to_string(l));
        pobierz_serwery_gry();
    }
}

void komunikat()
{
    auto& serwer = Kontekst::o().serwer;
    auto& klient = Kontekst::o().klient;

    if (serwer == nullptr && klient == nullptr)
    {
        printf("Zdecyduj czy jestes serwerem czy graczem:\n- jesli serwer to wpisz: 'serwer <nazwa_planszy>.txt' lub 'serwer <nr_planszy>'\njesli chcesz zagrac wiele razy na jednej mapie to na koncu podaj do ilu wygranych grasz\n w szczegolnosci to: string folder, string mapa, int do_ilu, double szybkosc, int ile_ludzi\n - jesli graczem to wpisz: 'klient <nazwa>'\n");
    }
    else if (klient != nullptr)
        komunikat_serwer_klient();
    else if (serwer != nullptr)
        komunikat_serwer_gry();
}

void wykonaj(string zadanie)
{
    LOG(INFO) << "Wykonuje: '" << zadanie << "'";

    auto& serwer = Kontekst::o().serwer;
    auto& klient = Kontekst::o().klient;

    if (serwer == nullptr && klient == nullptr)
    {
        if (zadanie.find("serwer") == 0)
            start_serwer_gry(zadanie);
        if (zadanie.find("klient") == 0)
            start_klient_gry(zadanie);
    }
    else if (klient != nullptr)
    {
        wykonaj_klient_gry(zadanie);
    }
    else if (serwer != nullptr)
    {
        wykonaj_serwer_gry(zadanie);
    }
    
    if (zadanie.find("odbierz") == 0)
    {
        vector<vector<string>> wiad;
        /*if (serwer != NULL)
        wiad = serwer->Odbierz();*/
        if (klient != NULL)
            wiad.push_back(Pobierz(*klient->wtyk).second);
        printf("Odebralem:\n");
        for (auto l : wiad)
        {
            for (auto s : l)
                printf("'%s'\n", s.c_str());
            printf("----\n");
        }
    }
    if (zadanie.find("napisz") == 0)
    {
        auto text = zadanie.substr(7);
        /*if (serwer != NULL)
            serwer->Rozeslij(text);
        if (klient != NULL)
            Wyslij(*klient->wtyk, text);*/
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

    auto serwer = Kontekst::o().serwer;
    auto klient = Kontekst::o().klient;

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
