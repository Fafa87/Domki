#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "../Domki/gui.h"

#include "../Domki/misja.h"
#include "../Domki/os.h"

#include "../Domki/multi.h"
#include "../Domki/multi_dzialacze.h"

#include "../Domki/Narzedzia/easylogging++.h" 
INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace multi;

Serwer* serwer = NULL;
Klient* klient = NULL;
MisjaUstawienia misja_ustawienia;
bool cichociemny = false;

#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

void wykonaj(string zadanie);
void konfiguruj(int l, const char * argv[])
{
    if (l == 0)
    {
        wykonaj("serwer " + string(argv[2]) + " " + string(argv[3]) + " " + string(argv[4]) + " " + string(argv[5]) + " " + string(argv[6]));
        while(misja_ustawienia.Zwyciezca() < 0)
            wykonaj("start");
        exit(0);
    }
    else if (l==1)
    { 
        wykonaj("klient " + std::to_string(l));
        klient->lista_serwerow.clear();
        klient->SpiszSerwery();
        //wykonaj("polacz");
    }
}

void komunikat()
{
    if (serwer == nullptr && klient == nullptr)
    {
        printf("Zdecyduj czy jestes serwerem czy graczem:\n- jesli serwer to wpisz: 'serwer <nazwa_planszy>.txt' lub 'serwer <nr_planszy>'\njesli chcesz zagrac wiele razy na jednej mapie to na koncu podaj do ilu wygranych grasz\n- jesli graczem to wpisz: 'klient <nazwa>'\n");
    }
    else if (klient != nullptr)
    {
        printf("Jestes graczem, musisz sie polaczyc z serwerm. Wpisz: 'polacz' lub 'polacz <adres_ip_serwera>'\n");
    }
    else if (serwer != nullptr)
    {
        printf("Jestes serwerem, gracze sie podlaczyli, napisz 'start' aby zaczac gre:\n");
    }
}

void wykonaj(string zadanie)
{
    if (serwer == nullptr && klient == nullptr)
    {
        if (zadanie.find("serwer") == 0)
        {
            serwer = new Serwer();
            auto adres = serwer->Postaw();
            printf("%s\n", adres.ToString().c_str());
            
            auto czastki = split(zadanie, ' ');

            for (int a = 0, b = czastki[1].size(); a < b; a++)if (czastki[1][a] == '+')czastki[1][a] = ' ';
            for (int a = 0, b = czastki[2].size(); a < b; a++)if (czastki[2][a] == '+')czastki[2][a] = ' ';

            auto misja_folder = czastki[1];
            auto misja_nazwa = czastki[2];
            if (misja_nazwa.size() == 1)
            {
                misja_nazwa = wczytaj_liste_plansz("Plansza\\"+ misja_folder)[atoi(misja_nazwa.c_str())];
            }
            auto misja_sciezka = "Plansza\\" + misja_folder + "\\" + czastki[2] ;

            // jak podana liczba wygranych to ja zapisz
            int liczba_gier = 1;
            if (czastki.size() >= 4) 
            {
                liczba_gier = atoi(czastki[3].c_str());
            }
            double predkosc = misja_ustawienia.szybkosc;
            if (czastki.size() >= 5)
            {
                predkosc = atof(czastki[4].c_str());
            }
            int liczba_graczy = misja_ustawienia.komputery.size() + 1;
            int liczba_ludzi = liczba_graczy;
            if (czastki.size() >= 6)
            {
                liczba_ludzi = atof(czastki[5].c_str());
            }

            misja_ustawienia = wczytaj_meta(misja_sciezka);
            misja_ustawienia.nazwa = misja_nazwa;
            misja_ustawienia.grupa = "Plansza\\" + misja_folder;
            misja_ustawienia.do_ilu_wygranych = liczba_gier;
            misja_ustawienia.ile_kto_wygranych = vector<int>(13);
            misja_ustawienia.szybkosc = predkosc;
            
            printf("Stworzona gra na planszy %s %s\n", misja_folder.c_str(), misja_nazwa.c_str());
            printf("Plansza na %d graczy\n", liczba_graczy);
            printf("Oczekuj na podlaczenie %d graczy\n", liczba_ludzi);
            printf("Gramy do %d\n", liczba_gier);

            misja_ustawienia.nazwy_graczow.push_back("Neutralny");
            for (int i = 0; i < liczba_ludzi; i++)
            {
                serwer->OczekujNaGracza();
                misja_ustawienia.nazwy_graczow.push_back(serwer->ludzie.back().nazwa);
            }
        }
        if (zadanie.find("klient") == 0)
        {
            klient = new Klient(zadanie.substr(7));
            printf("Klient: %s\n", klient->nazwa.c_str());
        }
    }
    else if (klient != nullptr)
    {
        if (zadanie.find("polacz") == 0)
        {
            Adres adres;
            if (klient->lista_serwerow.size() == 0)
            {
                auto cel = zadanie.substr(7);
                auto ip_port = split(cel, ':');
                adres = Adres(ip_port[0], stoi(ip_port[1]));
            }
            else
            {
                adres = klient->lista_serwerow.back();
            }
            klient->Podlacz(adres);

            // nie ma co czekac na gotowy
            //if (zadanie.find("gotowy") == 0)
            std::pair<sf::Socket::Status, MisjaUstawienia> res;
            do {
                printf("oczekuje na start misji... ");
                auto status_misja_ustawienia = klient->OczekujNaStart();
                auto misja_ustawienia = status_misja_ustawienia.second;
                if (status_misja_ustawienia.first != sf::Socket::Done)
                    LOG(WARNING) << "Odebranie ustawien misji buraka! " << status_misja_ustawienia.first;

                printf("startuje misje %s\n", misja_ustawienia.nazwa.c_str());

                string test = "A";
                klient->wtyk->setBlocking(false);

                KlientowyRuszacz ruszacz(*klient);

                misja_ustawienia.komputery.clear();
                misja(misja_ustawienia, ruszacz);

                printf("...misja skonczona\n");

                misja_ustawienia.WypiszRanking();
                
                klient->wtyk->setBlocking(true);
            } while (!(misja_ustawienia.Zwyciezca() >= 0));

            auto wygrany = misja_ustawienia.Zwyciezca();
            printf("\n=========================\nCaly mecz wygral: %s\n=========================\n", misja_ustawienia.nazwy_graczow[wygrany].c_str());
        }
    }
    else if (serwer != nullptr)
    {
        if (zadanie.find("start") == 0)
        {
            MisjaUstawienia ustawienia = misja_ustawienia;
            
            vector<int> komputery_mapy = ustawienia.komputery;
            ustawienia.komputery.clear();
            for (int i = ustawienia.nazwy_graczow.size()-2; i < komputery_mapy.size(); i++)
                ustawienia.komputery.push_back(komputery_mapy[i]);
            ustawienia.nr_gracza = 0;

            serwer->Start(ustawienia);

            //serwer->ludzie[0].wtyk->setBlocking(false);
            //serwer->ludzie[1].wtyk->setBlocking(false);

            SerwerowyRuszacz ruszacz(*serwer);

            misja(ustawienia, ruszacz);

            int liczba_gier = 0;
            for (auto w : ustawienia.ile_kto_wygranych)
                liczba_gier += w;

            printf("Rozegrano juz %d gier\n", liczba_gier);
            ustawienia.WypiszRanking();

            if (ustawienia.Zwyciezca() >= 0)
            {
                auto wygrany = ustawienia.Zwyciezca();
                printf("\n=========================\nCaly mecz wygral: %s\n=========================\n", ustawienia.nazwy_graczow[wygrany].c_str());

                delete serwer;
                serwer = nullptr;
            }
            misja_ustawienia = ustawienia;
        }
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

    try {
        do
        {
            komunikat();
            char tmp[1000];
            gets_s(tmp);
            zadanie = tmp;

            wykonaj(zadanie);

            if (klient != nullptr)
            {
                klient->lista_serwerow.clear();
                klient->SpiszSerwery();
            }

            Sleep(10);
        } while (zadanie != "k");
    }
    catch (const std::exception& ex)
    {
        LOG(ERROR) << ex.what();
        return -1;
    }

    return 0;
}
