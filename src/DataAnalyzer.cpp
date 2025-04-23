#include "DataAnalyzer.h"
#include <cmath>
#include <iostream>

DataAnalyzer::DataAnalyzer(HistogramManager& histManager) : histManager(histManager) {}

DataAnalyzer::~DataAnalyzer() {
    std::cout << "Finished with this run: " << RN << std::endl;
    std::cout << "Writing histograms to file..." << std::endl;
    writeHistograms("output/output_" + std::to_string(RN) + ".root");
}

void DataAnalyzer::analyzeEvent(hipo::bank& RECpart, hipo::bank& ALEtrk, hipo::bank& ALEadc, hipo::bank& ALEhit) {
    int PartNb = RECpart.getRows();
    int TrkNb = ALEtrk.getRows();
    if (PartNb == 0) return;

    for (int i = 0; i < PartNb; ++i) {
        if (RECpart.getInt("pid", i) != 22) continue;
        double p = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
                        RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
                        RECpart.getFloat("pz", i) * RECpart.getFloat("pz", i));
        if (p < 1.5) continue;
        double theta = acos(RECpart.getFloat("pz", i) / p);
        double phi = atan2(RECpart.getFloat("py", i), RECpart.getFloat("px", i));
        double q2 = 4.0 * 2.22 * p * pow(sin(theta / 2), 2);
        double nu = 2.22 - p;
        double Wp = sqrt(0.938 * 0.938 + 2 * 0.938 * nu - q2);
        double Wh = sqrt(1.875 * 1.875 + 2 * 1.875 * nu - q2);

        for (int j = 0; j < TrkNb; ++j) {
            double vz = ALEtrk.getFloat("z", j);
            double ep = sqrt(ALEtrk.getFloat("px", j) * ALEtrk.getFloat("px", j) +
                             ALEtrk.getFloat("py", j) * ALEtrk.getFloat("py", j) +
                             ALEtrk.getFloat("pz", j) * ALEtrk.getFloat("pz", j));
            double th = acos(ALEtrk.getFloat("pz", j) / ep);
            double ph = atan2(ALEtrk.getFloat("py", j), ALEtrk.getFloat("px", j));
            double dphi = ph - phi;
            double ppr = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
                              RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
                              (2.22 - RECpart.getFloat("pz", i)) * (2.22 - RECpart.getFloat("pz", i)));
            double edep = ALEtrk.getFloat("sum_adc", j);
            std::cout << "Energy deposition: " << edep << std::endl;

            histManager.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, ep, th, ph, dphi, ppr, edep);
        }
    }
}

void DataAnalyzer::writeHistograms(const std::string& outputFile) {
    histManager.writeHistograms(outputFile);
}

void DataAnalyzer::resetHistograms() {
    histManager.resetHistograms();
}

void DataAnalyzer::handleRunNumber(int RunNumber) {
    std::cout << "Handling RunNumber: " << RunNumber << std::endl;
    if (RN != 0) {
        std::cout << "Finished with this run: " << RN << std::endl;
        std::cout << "Writing histograms to file..." << std::endl;
        writeHistograms("output/output_" + std::to_string(RN) + ".root");
    }

    RN = RunNumber;
    std::cout << "RunNumber set to: " << RN << std::endl;
    resetHistograms();
}
