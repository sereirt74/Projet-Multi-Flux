#include <pnl/pnl_array.h>
#include <pnl/pnl_mathtools.h>
#include <pnl/pnl_random.h>
#include <pnl/pnl_cdf.h>
#include <pnl/pnl_integration.h>
#include <pnl/pnl_finance.h>
#include <pnl/pnl_root.h>
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_tridiag_matrix.h>
#include <BlackScholes.hpp>

BlackScholes::BlackScholes(int size_, PnlMat *sigma_, double r_, PnlVect *datesConstation_) {
    size = size_;
    r = r_;
    sigma = pnl_mat_copy(sigma_);
    datesConstation = pnl_vect_copy(datesConstation_);
}

BlackScholes::~BlackScholes() {
    pnl_mat_free(&sigma);
}

void BlackScholes::asset(PnlMat *path, const PnlMat *past, double dateActuelle, double T, int nbTimeSteps, PnlRng *rng) {
    double timeStep = T / nbTimeSteps;
    pnl_mat_set_subblock(path, past, 0, 0);
    int startStep = past->m;
    int m = datesConstation->size;
    PnlVect *normal_vect = pnl_vect_create(size);

    for (int i = startStep; i < nbTimeSteps; i++) {
        pnl_vect_rng_normal(normal_vect, size, rng);
        if (i > 0) {
            timeStep = (m == 1) ? GET(datesConstation, i - 1) - dateActuelle
                                : GET(datesConstation, i) - GET(datesConstation, i - 1);
        }
        for (int d = 0; d < size; d++) {
            PnlVect *vol_d = pnl_vect_new();
            pnl_mat_get_row(vol_d, sigma, d);
            double localSigma = pnl_vect_norm_two(vol_d);
            double growth = (r - (localSigma * localSigma) / 2) * timeStep;
            double diffusion = localSigma * sqrt(timeStep) * pnl_vect_get(normal_vect, d);
            double rate = growth + diffusion;
            double previousPrice = (i == 0) ? MGET(past, past->m - 1, d) : MGET(path, i - 1, d);
            MLET(path, i, d) = previousPrice * exp(rate);
            pnl_vect_free(&vol_d);
        }
    }

    pnl_vect_free(&normal_vect);
}

void BlackScholes::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double dateActuelle) {
    int m = datesConstation->size;
    int start_index = 0;
    while (start_index < m && GET(datesConstation, start_index) <= dateActuelle) {
        start_index++;
    }
    pnl_mat_set_subblock(shift_path, path, 0, 0);

    for (int i = start_index; i < path->m; i++) {
        double shifted_value = MGET(path, i, d) * (1 + h);
        pnl_mat_set(shift_path, i, d, shifted_value);
    }
}
