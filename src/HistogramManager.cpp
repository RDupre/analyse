#include "HistogramManager.h"
#include <cmath>
#include <iostream>

HistogramManager::HistogramManager() {
    // Initialize histograms
    PID = new TH1D("PID", "Particle ID", 100, -50, 50);
    Pel = new TH1D("Pel", "Pel", 100, 0, 2.5);
    Tel = new TH1D("Tel", "Tel", 100, 0, 0.5);
    Hel = new TH1D("Hel", "Hel", 100, -3.14, 3.14);
    Hq2 = new TH1D("q2", "q2", 100, 0, 1.0);
    Hnu = new TH1D("nu", "nu", 100, 0, 2.3);
    HWp = new TH1D("Wp", "Wp", 100, 0.7, 1.5);
    HWh = new TH1D("Wh", "Wh", 100, 1.4, 2.5);
    Hvz = new TH1D("Hvz", "Hvz", 100, -25, 25);
    Hep = new TH1D("Hep", "Hep", 100, 0, 1000);
    Hth = new TH1D("Hth", "Hth", 100, 0, 3.14);
    Hph = new TH1D("Hph", "Hph", 100, -3.14, 3.14);
    CPhi = new TH2D("DPhi", "DPhi", 60, -3.14, 3.14, 60, -3.14, 3.14);
    DelP = new TH1D("Delta Phi", "Delta Phi", 60, -3.14, 3.14);
    Ppc = new TH1D("Ppc", "Ppc", 100, 0, 0.8);
    Ecl = new TH1D("Ecl", "Ecl", 100, 0, 2.5);
    DPPc = new TH2D("DPhi vs Calc P", "DPhi2", 60, -3.14, 3.14, 60, 0, 0.8);
    PmPc = new TH2D("P meas vs calc", "PmPc", 60, 0, 600, 60, 0, 0.8);
}

HistogramManager::~HistogramManager() {
    // Write last histograms
    std::cout << "Finished with this run: " << RN << std::endl;
    std::cout << "Writing histograms to file..." << std::endl;
    writeHistograms("output/output_" + std::to_string(RN) + ".root");
    // Clean up histograms
    delete PID;
    delete Pel;
    delete Tel;
    delete Hel;
    delete Hq2;
    delete Hnu;
    delete HWp;
    delete HWh;
    delete Hvz;
    delete Hep;
    delete Hth;
    delete Hph;
    delete CPhi;
    delete DelP;
    delete Ppc;
    delete Ecl;
    delete DPPc;
    delete PmPc;
}

void HistogramManager::fillHistograms(const hipo::bank& RECpart, const hipo::bank& ALEtrk) {
    // Event counter 
    if(++evntCnt==100000) 
        std::cout << "Processing event number " << evntCnt<< std::endl;

    int PartNb = RECpart.getRows();
    int TrkNb = ALEtrk.getRows();
    if (PartNb == 0) return;

    for (int i = 0; i < PartNb; ++i) {
        if(RECpart.getInt("pid", i)!=22) continue;
        double p = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
                        RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
                        RECpart.getFloat("pz", i) * RECpart.getFloat("pz", i));
	if(p<1.5) continue;
        double theta = acos(RECpart.getFloat("pz", i) / p);
        double phi = atan2(RECpart.getFloat("py", i), RECpart.getFloat("px", i));
        double ppr = sqrt(RECpart.getFloat("px", i)*RECpart.getFloat("px", i) 
                        + RECpart.getFloat("py", i)*RECpart.getFloat("py", i) 
			+ (2.22-RECpart.getFloat("pz", i))*(2.22-RECpart.getFloat("pz", i)));
        double q2 = 4.0*2.22*p*pow(sin(theta/2), 2);
        double nu = 2.22 - p;
        double Wp = sqrt(0.938*0.938 + 2*0.938*nu - q2);
        double Wh = sqrt(1.875*1.875 + 2*1.875*nu - q2);
        Hq2->Fill(q2);
        Hnu->Fill(nu);
        HWp->Fill(Wp);
        HWh->Fill(Wh);
	if (Wp>1.5 || Wp<.86) continue;
        for (int i = 0; i < TrkNb; ++i) {
            double Avz = ALEtrk.getFloat("z", i);
            double Aep = sqrt(ALEtrk.getFloat("px", i)*ALEtrk.getFloat("px", i) + ALEtrk.getFloat("py", i)*ALEtrk.getFloat("py", i) + ALEtrk.getFloat("pz", i)*ALEtrk.getFloat("pz", i));
            double Ath = acos(ALEtrk.getFloat("pz", i)/p);
            double Aph = atan2(ALEtrk.getFloat("py", i), ALEtrk.getFloat("px", i));

            Hvz->Fill(Avz);
            Hep->Fill(Aep);
            Hth->Fill(Ath);
            Hph->Fill(Aph);
            Aph = Aph - 3.14/2; 
            if (Aph > 3.14) Aph = Aph - 2*3.14;
            if (Aph < -3.14) Aph = Aph + 2*3.14;
	    double Dph = Aph - phi;
            if (Dph > 3.14) Dph = Dph - 2*3.14;
            if (Dph < -3.14) Dph = Dph + 2*3.14;

            CPhi->Fill(Aph, phi);
            DelP->Fill(Dph);

            DPPc->Fill(Dph,ppr);
            if (Dph>1.5&&Dph<2.1) PmPc->Fill(Aep,ppr);
        }

        PID->Fill(RECpart.getInt("pid", i));
        Pel->Fill(p);
        Tel->Fill(theta);
        Hel->Fill(phi);
        Ppc->Fill(ppr);
    }
}

void HistogramManager::writeHistograms(const std::string& outputFile) {
    TFile file(outputFile.c_str(), "RECREATE");
    PID->Write();
    Pel->Write();
    Tel->Write();
    Hel->Write();
    Hq2->Write();
    Hnu->Write();
    HWp->Write();
    HWh->Write();
    Hvz->Write();
    Hep->Write();
    Hth->Write();
    Hph->Write();
    CPhi->Write();
    DelP->Write();
    Ppc->Write();
    Ecl->Write();
    DPPc->Write();
    PmPc->Write();
    file.Close();
}

void HistogramManager::resetHistograms() {
    PID->Reset();
    Pel->Reset();
    Tel->Reset();
    Hel->Reset();
    Hq2->Reset();
    Hnu->Reset();
    HWp->Reset();
    HWh->Reset();
    Hvz->Reset();
    Hep->Reset();
    Hth->Reset();
    Hph->Reset();
    CPhi->Reset();
    DelP->Reset();
    Ppc->Reset();
    Ecl->Reset();
    DPPc->Reset();
    PmPc->Reset();
}

void HistogramManager::handleRunNumber(int RunNumber) {
    std::cout << "Handling RunNumber: " << RunNumber << std::endl;
    // Example: Fill a histogram or perform other operations with RunNumber
    if(RN!=0) {
        std::cout << "Finished with this run: " << RN << std::endl;
        std::cout << "Writing histograms to file..." << std::endl;
        writeHistograms("output/output_" + std::to_string(RN) + ".root");
    } 

    RN = RunNumber;
    std::cout << "RunNumber set to: " << RN << std::endl;
    // Reset histograms or perform other operations as needed
    resetHistograms();
}
