#include "HistogramManager.h"
#include <cmath>
#include <iostream>

HistogramManager::HistogramManager(const std::string& ID) {
    // Initialize histograms
    PID  = new TH1D("PID_"+ID, "Particle ID", 100, -50, 50);
    Pel  = new TH1D("Pel_"+ID, "Pel", 100, 0, 2.5);
    Tel  = new TH1D("Tel_"+ID, "Tel", 100, 0, 0.5);
    Hel  = new TH1D("Hel_"+ID, "Hel", 100, -3.14, 3.14);
    Hq2  = new TH1D("q2__"+ID, "q2", 100, 0, 0.1);
    Hnu  = new TH1D("nu__"+ID, "nu", 100, 0, 2.3);
    HWp  = new TH1D("Wp__"+ID, "Wp", 100, 0.7, 1.5);
    HWh  = new TH1D("Wh__"+ID, "Wh", 100, 1.4, 2.5);
    Hvz  = new TH1D("Hvz_"+ID, "Hvz", 100, -50, 50);
    Hep  = new TH1D("Hep_"+ID, "Hep", 100, 0, 1000);
    Hth  = new TH1D("Hth_"+ID, "Hth", 100, 0, 3.14);
    Hph  = new TH1D("Hph_"+ID, "Hph", 100, -3.14, 3.14);
    CPhi = new TH2D("DPhi"+ID, "DPhi", 60, -3.14, 3.14, 60, -3.14, 3.14);
    DelP = new TH1D("DelP"+ID, "Delta Phi", 60, -3.14, 3.14);
    Ppc  = new TH1D("Ppc_"+ID, "Ppc", 100, 0, 0.8);
    Ecl  = new TH1D("Ecl_"+ID, "Ecl", 100, 0, 50000);
    DPPc = new TH2D("DPh2"+ID, "DPhi vs Calc P", 60, -3.14, 3.14, 60, 0, 0.8);
    PmPc = new TH2D("PmPc"+ID, "P calc vs Edep", 40, 0, 0.8, 50, 0, 50000);
}

HistogramManager::~HistogramManager() {
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

void HistogramManager::fillHistograms(double p, double theta, double phi, double q2, double nu, double Wp, double Wh,
                                      double vz, double ep, double th, double ph, double dphi, double ppr, double edep) {
    Hq2->Fill(q2);
    Hnu->Fill(nu);
    HWp->Fill(Wp);
    HWh->Fill(Wh);
    Hvz->Fill(vz);
    Hep->Fill(ep);
    Hth->Fill(th);
    Hph->Fill(ph);
    CPhi->Fill(phi, ph);
    DelP->Fill(dphi);
    Ppc->Fill(ppr);
    Pel->Fill(p);
    Tel->Fill(theta);
    Hel->Fill(phi);
    Ecl->Fill(edep);
    DPPc->Fill(dphi, ppr);
    PmPc->Fill(ppr,edep);
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

