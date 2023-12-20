#include "Option.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>


class CallVanille : public Option {
public:
    double K; // Prix d'exercice
    double T; // Maturité
    double r; // Taux sans risque
    int size; // Nombre d'actifs sous-jacents

    CallVanille(double T_, double K_, double r_, int size_);
    double payoff(const PnlMat *path);
    virtual ~CallVanille();
};
