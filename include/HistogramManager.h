#ifndef HISTOGRAMMANAGER_H
#define HISTOGRAMMANAGER_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include <string>

class HistogramManager {
public:
    HistogramManager();
    ~HistogramManager();

    void fillHistograms(double p, double theta, double phi, double q2, double nu, double Wp, double Wh,
                        double vz, double ep, double th, double ph, double dphi, double ppr);
    void writeHistograms(const std::string& outputFile);
    void resetHistograms();

private:
    TH1D* PID;
    TH1D* Pel;
    TH1D* Tel;
    TH1D* Hel;
    TH1D* Hq2;
    TH1D* Hnu;
    TH1D* HWp;
    TH1D* HWh;
    TH1D* Hvz;
    TH1D* Hep;
    TH1D* Hth;
    TH1D* Hph;
    TH2D* CPhi;
    TH1D* DelP;
    TH1D* Ppc;
    TH1D* Ecl;
    TH2D* DPPc;
    TH2D* PmPc;
};

#endif // HISTOGRAMMANAGER_H
