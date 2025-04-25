#include "DataAnalyzer.h"
#include <cmath>
#include <iostream>

DataAnalyzer::DataAnalyzer()
    // Initialize histogram managers
    : histManager0("_Trk")
    , histManager1("_Hi1")
    , histManager2("_Hi2")
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
        if (RECpart.getInt("pid", i) == 22 && theta > 0.09) continue;
        if (theta < 0.09) p *= 1.155; // Correct for sampling fraction in FT (or other problem?)
        if (p < 2. || p > 2.4) continue;
        double phi = atan2(RECpart.getFloat("py", i), RECpart.getFloat("px", i));
        double q2 = 4.0 * 2.22 * p * pow(sin(theta / 2), 2);
        double nu = 2.22 - p;
        double Wp = sqrt(0.938 * 0.938 + 2 * 0.938 * nu - q2);
        double Wh = sqrt(3.727 * 3.727 + 2 * 3.727 * nu - q2);
        if (Wp < 0.85 || Wp > 1.00) continue;
        double vz = RECpart.getFloat("vz", i);
	    if (vz < -16 || vz > 7) continue;
        //double ppr = sqrt(RECpart.getFloat("px", i) * RECpart.getFloat("px", i) +
        //                  RECpart.getFloat("py", i) * RECpart.getFloat("py", i) +
        //                  (2.22 - RECpart.getFloat("pz", i)) * (2.22 - RECpart.getFloat("pz", i)));
        double ppr = 2.22 * sin(theta);

        // Check tracks
        int TrkNb = ALEtrk.getRows();
        double chi2{1000000}, resi{0}, ep{0}, th{0}, ph{0}, dphi{0}, edep{0};
        int TrackId{0}, NbHitsOnTrack{0};
        for (int j = 0; j < TrkNb; ++j) {
            if (ALEtrk.getFloat("chi2", j) < chi2) chi2 = ALEtrk.getFloat("chi2", j);
            else continue;
	    TrackId = j + 1;
            resi = ALEtrk.getFloat("sum_residuals", j);
            ep = sqrt(ALEtrk.getFloat("px", j) * ALEtrk.getFloat("px", j) +
                      ALEtrk.getFloat("py", j) * ALEtrk.getFloat("py", j) +
                      ALEtrk.getFloat("pz", j) * ALEtrk.getFloat("pz", j));
            th = acos(ALEtrk.getFloat("pz", j) / ep);
            ph = atan2(ALEtrk.getFloat("py", j), ALEtrk.getFloat("px", j));
            dphi = ph - phi;
            if (dphi > 3.14159) dphi -= 2 * 3.14159;
            if (dphi < -3.14159) dphi += 2 * 3.14159;
            edep = (double)ALEtrk.getInt("sum_adc", j);
        }

        // Process hits from AHDC::hits
        int HitNb = ALEhit.getRows();
        int goodHit{0}, adcTot{0};
        double phiTot{0};
        for (int k = 0; k < HitNb; ++k) {
            int HitId = ALEhit.getShort("id", k);
            int layer = ALEhit.getInt("layer", k);
            int superlayer = ALEhit.getInt("superlayer", k);
            int wire = ALEhit.getInt("wire", k);
            int track_id = ALEhit.getInt("trackid", k);
            double time = ALEadc.getFloat("leadingEdgeTime", HitId-1);
            int adc = ALEadc.getInt("ADC", HitId-1);

            // Count hits on the track
            if (track_id == TrackId) NbHitsOnTrack+=1;

            // Calculate the wire number
            int WireNb = 0;
            switch (superlayer)
            {
            case 1: // 1st superlayer
                WireNb = 47;
                break;
            case 2: // 2nd superlayer
                WireNb = 56;
                break;
            case 3: // 3rd superlayer
                WireNb = 72;
                break;
            case 4: // 4th superlayer
                WireNb = 87;
                break;
            case 5: // 5th superlayer
                WireNb = 99;
                break;            
            default:
                std::cerr << "Invalid superlayer: " << superlayer << std::endl;
                continue;
            }

            // Calculate the wire phi position
            double wirePhiIndex = wire + 0.5*(WiresNb%2) + 0.5*layer*(1-2*(numWires%2)); 
            double wirePhi = wirePhiIndex * 2.0 * 3.14159 / WireNb;
            wirePhi += 0.35*vz*pow(-1, superlayer);
            if (wirePhi > 3.14159) wirePhi -= 2 * 3.14159;
            if (wirePhi < -3.14159) wirePhi += 2 * 3.14159;

            double wireDeltaPhi = wirePhi - phi;
            if (wireDeltaPhi > 3.14159) wireDeltaPhi -= 2 * 3.14159;
            if (wireDeltaPhi < -3.14159) wireDeltaPhi += 2 * 3.14159;

            if(ADC > 150)
                histManager1.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, 0, 0, wirePhi, wireDeltaPhi, ppr, adc, 0, 0);

            if (wireDeltaPhi < -2.0 || wireDeltaPhi > 2.0) {
                goodHit++;
                adcTot += adc;
                phiTot += wirePhi*adc;
            }

        }

        // Calculate the average wire phi position
        phiTot /= adcTot;
        wireDeltaPhi = phiTot - phi;
        if (wireDeltaPhi > 3.14159) wireDeltaPhi -= 2 * 3.14159;
        if (wireDeltaPhi < -3.14159) wireDeltaPhi += 2 * 3.14159;

        // Fill Histograms
        if (dphi < -2.0 || dphi > 2.0)
            histManager0.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, ep, th, ph, dphi, ppr, edep, chi2, resi);
        if(goodHit > 5) 
            histManager2.fillHistograms(p, theta, phi, q2, nu, Wp, Wh, vz, 0, 0, phiTot, wireDeltaPhi, ppr, adcTot, 0, 0);

    }
}

            void DataAnalyzer::writeHistograms(const std::string& outputFile) {
    histManager0.writeHistograms(outputFile + "_Trk.root");
    histManager1.writeHistograms(outputFile + "_Hi1.root");
    histManager2.writeHistograms(outputFile + "_Hi2.root");
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
