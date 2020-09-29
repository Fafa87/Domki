#pragma once

struct Czasy {
    double ile, czas, sred;
};

class Czasolicz
{
    enum Czas {
        FPS,
        APM,
        PING,
        OBLICZENIA
    };
    Czasy czasy[4];

    void init(Czas ktory) {
        int n = static_cast<int>(ktory);
        czasy[n].ile = 0.0;
        czasy[n].czas = 0.0;
        czasy[n].sred = 0.0;
    }
    void reset(Czas ktory) {
        int n = static_cast<int>(ktory);
        czasy[n].ile = 0.0;
        czasy[n].czas = 0.0;
        czasy[n].sred = 0.0;
    }
    void dodaj(Czas ktory, int ile, double czas) {
        int n = static_cast<int>(ktory);
        czasy[n].ile += ile;
        czasy[n].czas += czas;
        czasy[n].sred = ile/czas;
    }
    double wart(Czas ktory) { return czasy[static_cast<int>(ktory)].sred; }
};