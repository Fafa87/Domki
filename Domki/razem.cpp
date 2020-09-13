#include "razem.h"

#include "multi.h"
#include "multi_dzialacze.h"
#include "os.h"


string start_generuj_komende_startu(string folder, string mapa, int do_ilu, double szybkosc, int ile_ludzi, int port_serwera)
{
    for (int a = 0, b = folder.size(); a < b; a++)if (folder[a] == ' ')folder[a] = '+';
    for (int a = 0, b = mapa.size(); a < b; a++)if (mapa[a] == ' ')mapa[a] = '+';
    string parametry = "0 " + folder + " " + mapa + " " + to_string(do_ilu) + " " + to_string(szybkosc) + " " + to_string(ile_ludzi) + " " + to_string(port_serwera);
    return parametry;
}

PROCESS_INFORMATION start_nowej_gry_dla_wielu(string parametry)
{
    LOG(INFO) << "Startujemy serwer " + parametry;
    string komenda = "MultiSerwer.exe " + parametry;

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;
    if (!CreateProcess("MultiSerwer.exe", _strdup(komenda.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
        throw exception("Nie udalo sie odpalic serwera.");
    return processInfo;
}

void start_klient(sf::Music& muzyka, string nazwa, multi::Adres adres)
{
    auto klient = new multi::Klient(nazwa);
    LOG(INFO) << "Klient: " << klient->nazwa.c_str();

    Sleep(3);
    if (adres.ip.size() == 0)
    {
        LOG(TRACE) << "Spisuje serwery";
        klient->SpiszSerwery();

        multi::Adres adres;
        if (klient->lista_serwerow.size() == 0)
            LOG(WARNING) << "Brak serwera";
        else
            adres = klient->lista_serwerow.back();
    }

    LOG(INFO) << "Znaleziony serwer: " << adres.ToString();
    klient->Podlacz(adres);

    std::pair<bool, MisjaUstawienia> res;
    do {
        if (!klient->WyslijGotowosc())
            break;

        LOG(INFO) << "Oczekuje na start misji...";
        res = klient->OczekujNaStart();
        if (res.first != sf::Socket::Done)
            LOG(WARNING) << "Odebranie ustawien misji buraka! " << res.first;

        LOG(INFO) << "Startuje misje " << res.second.nazwa;

        string test = "A";
        klient->wtyk->setBlocking(false);

        KlientowyRuszacz ruszacz(*klient);

        res.second.komputery.clear();
        misja(res.second, ruszacz);

        if (!ruszacz.gotowy) // nie doczekal sie
            break;

        LOG(INFO) << "... misja skonczona";

        res.second.WypiszRanking();

        klient->wtyk->setBlocking(true);
    } while (!(res.second.Zwyciezca() >= 0));

    klient->Rozlacz();

    while (GUI::aplikacja().zalozone_gry.size())
    {
        TerminateProcess(GUI::aplikacja().zalozone_gry.back().hProcess, -2);
        GUI::aplikacja().zalozone_gry.pop_back();
    }
}