#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"

class BlackScholes {
public:
    int size;
    PnlVect *paymentDates;
    double r;
    PnlMat *sigma; // Modifié à PnlMat*

    BlackScholes(int size_, PnlMat *sigma_, double r_, PnlVect *datesConstation_);
    ~BlackScholes();

    //void asset_0(PnlMat* path, int timestep, PnlRng* rng, double T);


    void asset(PnlMat *path, const PnlMat *past, double dateActuelle, bool isMonitoringDate, double T, int nbTimeSteps, PnlRng *rng);
    // t est la date actuelle
    void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double dateactuelle);
};