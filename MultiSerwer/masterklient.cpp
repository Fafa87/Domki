#include "../MultiSerwer/mastery.h"
#include "../Domki/ext_string.h"
#include "../Domki/ext_vector.h"


int komunikat_ostatni = -1;
void komunikat_masterklient(mastery::Klient* klient)
{
    if ((int)klient->polaczony != komunikat_ostatni)
    {
        if (klient->polaczony)
            printf("Jestes juz polaczony, mozesz:\n- napisac tekst\n- rozlacz\n- /KTO? jest w pokoju serwera\n- /IDZ: do pokoju\n- /START: parametry serwera\n");
        else
            printf("Nie jestes jeszcze polaczony do serwera. Napisz polacz <adres>:<port>, aby sprobowac sie polaczyc do msa.\n");
    }
    komunikat_ostatni = (int)klient->polaczony;
}

void start_masterklient(string nazwa)
{
    KontekstSwiata::o().klient = new mastery::Klient(nazwa);
    auto& klient = KontekstSwiata::o().klient;
}

void wykonaj_masterklient(mastery::Klient* klient, string zadanie)
{
    if (zadanie.find("polacz") == 0)
    {
        auto adres_port = zadanie.substr(7);
        auto ip_port = split(adres_port, ':');
        if (ip_port[0].size() == 0)
        {
            ip_port[0] = mastery::ADRES_MASTER;
        }
        auto adres = multi::Adres(ip_port[0], stoi(ip_port[1]));

        std::thread([&klient, adres]() { klient->Podlacz(adres); }).detach();
    }
    else if (zadanie == "rozlacz")
    {
        klient->Rozlacz();
    }
    else if (zadanie.find("/KTO?") == 0)
    {
        klient->KtoJest();
    }
    else if (zadanie.find("/IDZ: ") == 0)
    {
        auto pokoj = zadanie.substr(6);
        klient->IdzDo(pokoj);
    }
    else if (zadanie.size() > 0)
    {
        // mowi cos
        klient->komendy.add(zadanie);
    }
}

vector<string> mastery::Klient::KtoJestObok()
{
    if(this->ludzie_obok.size()==0 && oczekuje_na_liste == false)
    {
        // zapytaj serwer
        oczekuje_na_liste = true;
        KtoJest();
    }
    return this->ludzie_obok;
}

void mastery::Klient::Rozlacz()
{
    if (this->polaczony)
    {
        LOG(INFO) << "Odlaczam sie od " << this->adres;
        this->polaczony = false;
        this->adres_serwer = Adres();
        this->ludzie_obok.clear();
        this->rozgrywka_pokoju = Adres();
    }
}

void mastery::Klient::KtoJest()
{
    komendy.add("/KTO?");
}

void mastery::Klient::IdzDo(string pokoj)
{
    komendy.add("/IDZ: " + pokoj);
    this->ludzie_obok.clear();
    this->rozgrywka_pokoju = Adres();
}

void mastery::Klient::PrzeanalizujOdebrane(string tekst)
{
    LOG(INFO) << "Odebralem: " << tekst;
    // TMP sprawdzane po tekscie -> panie to walnie!
    if (tekst.find("Drogi") == 0 && oczekuje_na_liste) // lista ludzi 
    {
        auto linie = split(tekst, '\n');
        linie.erase(linie.begin());
        this->ludzie_obok = linie;
        oczekuje_na_liste = false;
    }
    else if (tekst.find(" wchodzi") != -1)
    {
        auto nazwa = tekst.substr(0, tekst.find(" wchodzi"));
        this->ludzie_obok.push_back(nazwa); // TMP miejmy nadzieje ze to sie z nikim nie zderzy
        odebrane.add(nazwa + " wkroczyl do pokoju");
    }
    else if (tekst.find(" opuszcza") != -1)
    {
        auto nazwa = tekst.substr(0, tekst.find(" opuszcza"));
        remove_item(this->ludzie_obok, nazwa); // TMP miejmy nadzieje ze to sie z nikim nie zderzy
        odebrane.add(nazwa + " opuscil pokoj");
    }
    else if (tekst.find("na porcie ") != -1)
    {
        auto port = stoi(tekst.substr(tekst.find(" na porcie ")+11));
        this->rozgrywka_pokoju = Adres(this->adres_serwer.ip, port);
        odebrane.add("Odpalono rozgrywke!");
    }
    else
        odebrane.add(tekst);
}

void mastery::Klient::Podlacz(multi::Adres adres)
{
    sf::Socket::Status status = gracz.wtyk->connect(adres.ip, adres.port, sf::seconds(5));
    this->polaczony = false;
    gracz.ostatnio = status;
    if (status != sf::Socket::Done)
    {
        LOG(WARNING) << "Nie udalo sie polaczyc: " << status;
    }
    else
    {
        LOG(INFO) << "Polaczony z: " << adres.ToString();
        status = multi::Wyslij(*gracz.wtyk, gracz.nazwa);
        gracz.ostatnio = status;
        if (status != sf::Socket::Done)
        {
            LOG(WARNING) << "Nazwa nie dotarla: " << status;
            gracz.wtyk->disconnect();
        }
        else
        {
            LOG(INFO) << "Imie wyslane i przyjete.";
            this->polaczony = true;
            this->adres_serwer = adres;

            // obsluguj polaczenie
            while (this->polaczony)
            {
                // jesli co jest do powiedzenia to to napisz
                {
                    string tekst;
                    auto status = komendy.try_take(tekst);
                    if (status == code_machina::BlockingCollectionStatus::Ok)
                    {
                        LOG(INFO) << "Napisalem: " << tekst;
                        auto status_wyslania = multi::Wyslij(*gracz.wtyk, tekst);
                        if (status_wyslania == sf::Socket::Disconnected)
                        {
                            LOG(INFO) << "Serwer mnie odrzucil...";
                            Rozlacz();
                        }
                    }
                }

                // jesli cos przyszlo to to wypisz
                {
                    auto status_dane = multi::Pobierz(*gracz.wtyk, sf::seconds(0.1));
                    if (status_dane.first == sf::Socket::Done)
                    {
                        PrzeanalizujOdebrane(status_dane.second[0]); // TMP czy tutaj nie powinienem braæ wszystkich, a nie tylko pierwszy element
                    }
                }

                Sleep(100);
            }

            gracz.wtyk->disconnect();
        }
    }
}