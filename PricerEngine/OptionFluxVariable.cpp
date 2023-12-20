#include <cmath>
#include <iostream>
#include <vector>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include <cmath>
#include "OptionFluxVariable.hpp"
#include <random>


using namespace std;


OptionFluxVariable::OptionFluxVariable(double maturite_, int nbPasTemps_, int dimension_, double taux_, PnlVect *strikes_, PnlVect *datesFlux_) {
    maturite = maturite_;
    nbPasTemps = nbPasTemps_;
    dimension = dimension_;
    taux = taux_;
    strikes = pnl_vect_copy(strikes_);
    datesFlux = pnl_vect_copy(datesFlux_);
}

double OptionFluxVariable::payoff(const PnlMat *chemin) {
    double resultat = 0.0;
    double exponentielle = 0.0;
    for (int i = 0; i < datesFlux->size; i++) {
        exponentielle = exp(taux * (maturite - GET(datesFlux, i)));
        resultat += exponentielle * max(pnl_mat_get(chemin,i,i ) - GET(strikes, i), 0.0);
    }
    return resultat;
}

OptionFluxVariable::~OptionFluxVariable() {
    pnl_vect_free(&strikes);
    pnl_vect_free(&datesFlux);
}

//main pour tester l'option

//int main() {
//    double maturite = 1.0;
//    int nbPasTemps = 12;
//    int dimension = 1;
//    double taux = 0.05;
//    PnlVect *prixExercices = pnl_vect_create_from_list(3, 100.0, 105.0, 110.0);
//    PnlVect *datesFlux = pnl_vect_create_from_list(3, 0.25, 0.5, 0.75); // Flux à 3, 6 et 9 mois
//
//    OptionFluxVariable option(maturite, nbPasTemps, dimension, taux, prixExercices, datesFlux);
//
//    //generer trajectoire
//    PnlMat *chemin = pnl_mat_create(nbPasTemps + 1, dimension);
//    for (int i = 0; i < nbPasTemps; ++i) {
//        double valeur = 90 + (rand() % 21); 
//        pnl_mat_set(chemin, i, 0, valeur);
//    }
//    pnl_mat_set(chemin, nbPasTemps, 0, 110); // Dernière valeur à 110
//
//    double payoff = option.payoff(chemin);
//    cout << "Payoff de l'Option avec plusieurs flux: " << payoff << endl;
//
//    pnl_vect_free(&prixExercices);
//    pnl_vect_free(&datesFlux);
//    pnl_mat_free(&chemin);
//
//    return 0;
//}

