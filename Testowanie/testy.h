#pragma once

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>

int pracuj()
{
    int w = 2;
    for (int i = 0; i < 100000000; i++)
    {
        w = w * w;
    }
    return w;

}

int testuj_czasy()
{
    int ile = 100000000;
    int wynik = 0;

    auto start = clock();
    wynik = pracuj();
    Sleep(1000);
    auto czas = clock() - start;
    printf("Wynik pracy=%d, czas=%ld ms\n", wynik, czas * 1000 / CLOCKS_PER_SEC);


    start = clock();
    long end;
    for (int i = 0; i < ile; i++)
        end = clock();
    printf("clock() caly=%lf ms\n", (end - start) * 1000.0 / CLOCKS_PER_SEC);
    printf("clock() zajal=%lf ms\n", (end - start) * 1000.0 / CLOCKS_PER_SEC / ile);
    printf("clock()=%ld\n", end);

    auto start2 = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point end2;
    for (int i = 0; i < ile; i++)
        end2 = std::chrono::system_clock::now();
    Sleep(3000);
    printf("system_clock() caly=%lf ms\n", (end2 - start2).count());
    printf("system_clock() zajal=%lf ms\n", (end2 - start2).count() / 1000.0 / ile);
    printf("system_clock()=%lf\n", end2.time_since_epoch().count());


    return 0;

}