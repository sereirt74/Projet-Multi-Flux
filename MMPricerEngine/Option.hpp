#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option {
public:
    double T;        /// maturit�
    int nbTimeSteps; /// nombre de pas de temps de discr�tisation
    int size;        /// dimension du mod�le, redondant avec BlackScholesModel::size_
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du mod�le telle que cr��e
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;
};
