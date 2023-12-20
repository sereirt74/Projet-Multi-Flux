#include <iostream>
#include "json_reader.hpp"
#include "pricer.hpp"
#include "MonteCarlo.hpp"
#include "Option.hpp"
//#include "OptionFluxVariable.hpp"


BlackScholesPricer::BlackScholesPricer(nlohmann::json &jsonParams) {
    jsonParams.at("VolCholeskyLines").get_to(volatility);
    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("Strikes").get_to(strikes);
    jsonParams.at("DomesticInterestRate").get_to(interestRate);
    jsonParams.at("RelativeFiniteDifferenceStep").get_to(fdStep);
    jsonParams.at("SampleNb").get_to(nSamples);

    //------------------------
    nAssets = volatility->n;
    T = pnl_vect_get(paymentDates, nAssets - 1);
    //int nbPasTemps_ = 12;
    opt = new OptionFluxVariable(T, nAssets, nAssets, interestRate, strikes, paymentDates);

    model = new BlackScholes(nAssets, volatility,  interestRate, paymentDates);
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    //mc = new MonteCarlo(mod, opt, rng, fdStep, nSamples);
    //////////////////////
    
}


BlackScholesPricer::~BlackScholesPricer() {

    pnl_vect_free(&paymentDates);
    pnl_vect_free(&strikes);
    pnl_mat_free(&volatility);
    pnl_rng_free(&rng);
}

void BlackScholesPricer::print() {
    std::cout << "nAssets: " << nAssets << std::endl;
    std::cout << "fdStep: " << fdStep << std::endl;
    std::cout << "nSamples: " << nSamples << std::endl;
    std::cout << "strikes: ";
    pnl_vect_print_asrow(strikes);
    std::cout << "paymentDates: ";
    pnl_vect_print_asrow(paymentDates);
    std::cout << "volatility: ";
    pnl_mat_print(volatility);
}


void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect *&deltas, PnlVect *&deltasStdDev) {
    // Initialisation des variables de sortie
    price = 0.0;
    priceStdDev = 0.0;
    deltas = pnl_vect_create_from_zero(nAssets);
    deltasStdDev = pnl_vect_create_from_zero(nAssets);

    PnlMat *path = pnl_mat_create(opt->nbPasTemps, nAssets);
    PnlMat *shiftPath = pnl_mat_create(opt->nbPasTemps, nAssets);

    double esp = 0.0, esp2 = 0.0;
    double delta_d, payoff, payoff_plus, payoff_minus;

    for (int j = 0; j < nSamples; ++j) {
        model->asset(path, past, currentDate, isMonitoringDate, T, nAssets, rng);
        payoff = opt->payoff(path);
        esp += payoff;
        esp2 += payoff * payoff;

        for (int d = 0; d < nAssets; ++d) {
            model->shiftAsset(shiftPath, path, d, fdStep, currentDate);
            payoff_plus = opt->payoff(shiftPath);
            model->shiftAsset(shiftPath, path, d, -fdStep, currentDate);
            payoff_minus = opt->payoff(shiftPath);
            delta_d = (payoff_plus - payoff_minus) / (2 * fdStep);
            LET(deltas, d) += delta_d;
            LET(deltasStdDev, d) += delta_d * delta_d;
        }
    }

    double exprT_t = exp(-model->r * (T - currentDate));
    esp /= nSamples;
    esp2 /= nSamples;
    price = exprT_t * esp;
    priceStdDev = sqrt(abs((exprT_t * exprT_t * esp2 - price * price) / nSamples));

    double espDelta = exprT_t / (2 * fdStep * nSamples);
    double esp2Delta = espDelta * espDelta * nSamples;
    double st, fact;
    for (int d = 0; d < nAssets; ++d) {
        st = pnl_mat_get(past, past->m - 1, d);
        delta_d = GET(deltas, d);
        LET(deltas, d) = delta_d * espDelta / st;
        fact = GET(deltasStdDev, d) * (esp2Delta / (st * st)) - pnl_pow_i(delta_d / (2 * fdStep * nSamples * st), 2);
        LET(deltasStdDev, d) = sqrt(abs(fact) / nSamples);
    }

    pnl_mat_free(&path);
    pnl_mat_free(&shiftPath);
}





//void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect *&deltas, PnlVect *&deltasStdDev) {
//    price = 0.0;
//    priceStdDev = 0.0;
//    deltas = pnl_vect_create_from_zero(nAssets);
//    deltasStdDev = pnl_vect_create_from_zero(nAssets);
//
//    PnlMat *path = pnl_mat_create(opt->nbTimeSteps, nAssets);
//    PnlMat *shiftPath = pnl_mat_create(opt->nbTimeSteps, nAssets);
//
//    double payoff, variance_payoff = 0.0;
//    double delta_d, variance_delta_d;
//
//    for (int j = 0; j < nSamples; j++) {
//        model->asset(path, past, currentDate, isMonitoringDate, opt->nbTimeSteps, T, rng);
//        payoff = opt->payoff(path);
//        price += payoff;
//        variance_payoff += payoff * payoff;
//
//        for (int d = 0; d < nAssets; d++) {
//            model->shiftAsset(shiftPath, path, d, fdStep, currentDate, isMonitoringDate);
//            double payoff_plus = opt->payoff(shiftPath);
//            model->shiftAsset(shiftPath, path, d, -fdStep, currentDate, isMonitoringDate);
//            double payoff_minus = opt->payoff(shiftPath);
//
//            delta_d = (payoff_plus - payoff_minus) / (2 * fdStep * MGET(past, past->m - 1, d));
//            LET(deltas, d) += delta_d;
//            variance_delta_d = delta_d * delta_d;
//            LET(deltasStdDev, d) += variance_delta_d;
//        }
//    }
//
//    double discountFactor = exp(-model->interestRate * (T - currentDate));
//    price = discountFactor * (price / nSamples);
//    priceStdDev = sqrt((discountFactor * discountFactor * (variance_payoff / nSamples) - price * price) / nSamples);
//
//    for (int d = 0; d < nAssets; d++) {
//        LET(deltas, d) = discountFactor * (GET(deltas, d) / nSamples);
//        LET(deltasStdDev, d) = sqrt((discountFactor * discountFactor * (GET(deltasStdDev, d) / nSamples) - GET(deltas, d) * GET(deltas, d)) / nSamples);
//    }
//
//    pnl_mat_free(&path);
//    pnl_mat_free(&shiftPath);
//}
