#ifndef HISTOGRAMMANAGER_H
#define HISTOGRAMMANAGER_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "hipo4/bank.h"

class HistogramManager {
public:
    HistogramManager();
    ~HistogramManager();
    void fillHistograms(hipo::bank& RECpart, hipo::bank& ALEtrk);
    void writeHistograms(const std::string& outputFile);

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
};

#endif // HISTOGRAMMANAGER_H