#include "Option.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>

using namespace std;

class OptionFluxVariable : public Option {
public:
    double taux;    
    PnlVect *strikes;  
    PnlVect *datesFlux; 
    OptionFluxVariable(double maturite_, int nbPasTemps_, int dimension_, double taux_, PnlVect *strikes_, PnlVect *datesFlux_);
    double payoff(const PnlMat *chemin);
    virtual ~OptionFluxVariable();
};
