#ifndef HISTOGRAMMANAGER_H
#define HISTOGRAMMANAGER_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "hipo4/bank.h"
#include <string>

class HistogramManager {
public:
    HistogramManager();
    ~HistogramManager();
    void fillHistograms(const hipo::bank& RECpart, const hipo::bank& ALEtrk);
    void writeHistograms(const std::string& outputFile);
    void resetHistograms();
    void handleRunNumber(int RunNumber); // Updated method to handle RunNumber

private:
    int RN=0;
    int evntCnt = 0;
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
