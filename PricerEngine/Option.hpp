#pragma once
#include <stdio.h>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

class Option {
public:
    double maturite; 
    int nbPasTemps; 
    int dimension;  
    virtual double payoff(const PnlMat *chemin) = 0;
};
