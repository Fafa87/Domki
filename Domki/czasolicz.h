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


    void init(enum czas) {}
    void reset(enum czas) {}
    void dodaj(enum czas, int ile, double czas) {}
    double wart(enum czas) { return 0.0; }
};