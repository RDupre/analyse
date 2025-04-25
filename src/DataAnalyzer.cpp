#include "DataAnalyzer.h"
#include <cmath>
#include <iostream>

DataAnalyzer::DataAnalyzer()
    // Initialize histogram managers
    : histManager0("_All")
    , histManager1("_FTe")
    , histManager2("_FDe")
{
    // Initialize run number
    RN = 0; 
}

DataAnalyzer::~DataAnalyzer() {
    std::cout << "Finished with this run: " << RN << std::endl;
    std::cout << "Writing histograms to file..." << std::endl;
    writeHistograms("output/output_" + std::to_string(RN));
}

void DataAnalyzer::analyzeEvent(hipo::bank& RECpart, hipo::bank& ALEtrk, hipo::bank& ALEadc, hipo::bank& ALEhit) {
    int PartNb = RECpart.getRows();
    if (PartNb == 0) return;

    for (int i = 0; i < PartNb; ++i) {
        if (RECpart.getInt("pid", i) != 22 && RECpart.getInt("pid", i) != 11) continue;
        double p = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
                        RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
                        RECpart.getFloat("pz", i) * RECpart.getFloat("pz", i));
        double theta = acos(RECpart.getFloat("pz", i) / p);
        if (RECpart.getInt("pid", i) == 22 && theta>0.09) continue;
        if (theta<0.09) p *= 1.155; // Correct for sampling fraction in FT (or other problem?)
        if (p < 2. || p > 2.4) continue;
        double phi = atan2(RECpart.getFloat("py", i), RECpart.getFloat("px", i));
        double q2 = 4.0 * 2.22 * p * pow(sin(theta / 2), 2);
        double nu = 2.22 - p;
        double Wp = sqrt(0.938 * 0.938 + 2 * 0.938 * nu - q2);
        double Wh = sqrt(3.727 * 3.727 + 2 * 3.727 * nu - q2);
        if(Wp<0.85||Wp>1.00) continue;
        double vz = RECpart.getFloat("vz", i);
        double ppr = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
                          RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
                          (2.22 - RECpart.getFloat("pz", i)) * (2.22 - RECpart.getFloat("pz", i)));
        
	// Check tracks
        int TrkNb = ALEtrk.getRows();
	double chi2{1000000}, resi{0}, ep{0}, th{0}, ph{0}, dphi{0}, edep{0};
        for (int j = 0; j < TrkNb; ++j) {
            if(ALEtrk.getFloat("chi2", j) < chi2) chi2 = ALEtrk.getFloat("chi2", j);
	    else continue;
            resi = ALEtrk.getFloat("sum_residuals", j);
            ep = sqrt(ALEtrk.getFloat("px", j) * ALEtrk.getFloat("px", j) +
                      ALEtrk.getFloat("py", j) * ALEtrk.getFloat("py", j) +
                      ALEtrk.getFloat("pz", j) * ALEtrk.getFloat("pz", j));
            th = acos(ALEtrk.getFloat("pz", j) / ep);
            ph = atan2(ALEtrk.getFloat("py", j), ALEtrk.getFloat("px", j));
            dphi = ph - phi;
            if (dphi> 3.14) dphi-=2*3.14;
            if (dphi<-3.14) dphi+=2*3.14;
            edep = (double) ALEtrk.getInt("sum_adc", j);
        }
	if (dphi<-2.7 || dphi>2.8){ 
	// Fill Histograms
            histManager0.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, ep, th, ph, dphi, ppr, edep, chi2, resi);
	    if(theta<0.09)
            histManager1.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, ep, th, ph, dphi, ppr, edep, chi2, resi);
	    if(theta>=0.09)
            histManager2.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, ep, th, ph, dphi, ppr, edep, chi2, resi);
	}
    }
}

void DataAnalyzer::writeHistograms(const std::string& outputFile) {
    histManager0.writeHistograms(outputFile + "_All.root");
    histManager1.writeHistograms(outputFile + "_FTe.root");
    histManager2.writeHistograms(outputFile + "_FDe.root");
}

void DataAnalyzer::resetHistograms() {
    histManager0.resetHistograms();
    histManager1.resetHistograms();
    histManager2.resetHistograms();
}

void DataAnalyzer::handleRunNumber(int RunNumber) {
    std::cout << "Handling RunNumber: " << RunNumber << std::endl;
    if (RN != 0) {
        std::cout << "Finished with this run: " << RN << std::endl;
        std::cout << "Writing histograms to file..." << std::endl;
        writeHistograms("output/output_" + std::to_string(RN));
    }

    RN = RunNumber;
    std::cout << "RunNumber set to: " << RN << std::endl;
    resetHistograms();
}
