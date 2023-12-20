#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "pnl/pnl_cdf.h"
#include "MonteCarlo.hpp"
#include "BlackScholes.hpp"
#include "Option.hpp"
#include "OptionFluxVariable.hpp"

class BlackScholesPricer {
public:
    PnlMat *volatility;
    PnlVect *paymentDates;
    PnlVect *strikes;
    int nAssets;
    double interestRate;
    double fdStep;
    int nSamples;
    double T;            // Ajout de T

    BlackScholes* model; // Ajout de model
    OptionFluxVariable *opt; // Ajout de opt
    PnlRng* rng;         // Ajout de rng


    BlackScholesPricer(nlohmann::json &jsonParams);

    ~BlackScholesPricer();
    void priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect *&deltas, PnlVect *&deltasStdDev);
    void print();
};
