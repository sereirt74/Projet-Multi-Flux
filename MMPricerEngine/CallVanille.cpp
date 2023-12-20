#include "CallVanille.hpp"
#include <cmath>
#include <iostream>
#include <algorithm> // Ajoutez cette ligne

using namespace std;

CallVanille::CallVanille(double T_, double K_, double r_, int size_) {
    T = T_;
    K = K_;
    r = r_;
    size = size_;
}

double CallVanille::payoff(const PnlMat *path) {
    double spot = pnl_mat_get(path, path->m - 1, 0); // On prend le dernier prix de l'actif sous-jacent
    return max(spot - K, 0.0) * exp(-r * T);         // Formule de payoff pour un call vanille
}

CallVanille::~CallVanille() {
    // Pas de mémoire dynamique allouée, donc pas besoin de libérer
}