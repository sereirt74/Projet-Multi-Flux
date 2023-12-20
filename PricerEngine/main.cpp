#include <iostream>
#include <pnl/pnl_random.h>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_matrix.h>
#include "BlackScholes.hpp"

using namespace std;

int main() {
    // Définition des paramètres du modèle Black-Scholes
    int nAssets = 3; // Nombre d'actifs
    double interestRate = 0.05; // Taux d'intérêt
    double T = 1.0; // Maturité
    int nbTimeSteps = 10; // Nombre de pas de temps

    // Création de la matrice de volatilité
    PnlMat* volatility = pnl_mat_create_from_scalar(nAssets, nAssets, 0.2);

    // Création du vecteur des dates de paiement (paymentDates)
    PnlVect* paymentDates = pnl_vect_create_from_double(nbTimeSteps, T/nbTimeSteps);

    // Instanciation du modèle Black-Scholes
    BlackScholes blackScholes(nAssets, volatility, interestRate, paymentDates);

    // Création d'une matrice pour stocker le chemin des actifs
    PnlMat* path = pnl_mat_create(nbTimeSteps + 1, nAssets);

    // Création d'une matrice 'past' avec des données initiales
    PnlMat* past = pnl_mat_create(nbTimeSteps/2 + 1, nAssets);
    pnl_mat_set_double(past, 100.0); // Initialisation avec une valeur constante

    // Création d'un générateur aléatoire
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
    PnlRng* rng2 = pnl_rng_create(PNL_RNG_MERSENNE);

    int specificSeed = 123456; // Exemple de graine
    pnl_rng_sseed(rng2, specificSeed);
    // Appel de la fonction asset
    blackScholes.asset(path, past, 0.0, false, T, nbTimeSteps, rng2);

    // Affichage du chemin généré
    pnl_mat_print(path);

    // Libération de la mémoire
    pnl_mat_free(&volatility);
    pnl_vect_free(&paymentDates);
    pnl_mat_free(&path);
    pnl_mat_free(&past);
    pnl_rng_free(&rng2);

    return 0;
}
















/*
// vim: set sw=4 ts=4 sts=4:

#include <iostream>
#include <ctime>
#include "BSScheme.hpp"
#include "pnl/pnl_random.h"

int main()
{
    const int n = 100; //nb pas de temps
    double T = 1.0; // Maturité
    double S0 = 100.0;
    double r = 0.05; // Taux d'intérêt
    double sigma = 0.2; //volatilité

    // Initialisation du générateur de nombres aléatoires
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    // Création des instances de BSEuler et des vecteurs
    BSEuler euler(T, sigma, r, S0);
    PnlVect *path = pnl_vect_create(n + 1); // +1 car on inclut le temps 0
    PnlVect *G = pnl_vect_new();

    // Génération des réalisations du mouvement Brownien
    pnl_vect_rng_normal(G, n, rng);
    // Génération de la trajectoire
    euler.simul(path, G, n);

    // Affichage de la trajectoire
    for (int i = 0; i <= n; i++) {
        std::cout << "S(" << i << ") = " << GET(path, i) << std::endl;
    }

    // Libération des ressources
    pnl_vect_free(&path);
    pnl_vect_free(&G);
    pnl_rng_free(&rng);

    return 0;
}
*/
