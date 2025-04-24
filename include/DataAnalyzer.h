#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include "hipo4/bank.h"
#include "HistogramManager.h"

class DataAnalyzer {
public:
    DataAnalyzer();
    ~DataAnalyzer();
    void analyzeEvent(hipo::bank& RECpart, hipo::bank& ALEtrk, hipo::bank& ALEadc, hipo::bank& ALEhit);
    void writeHistograms(const std::string& outputFile);
    void resetHistograms();
    void handleRunNumber(int RunNumber);

private:
    int RN = 0;
    HistogramManager& histManager0; // First histogram manager
    HistogramManager& histManager1; // First histogram manager
    HistogramManager& histManager2; // Second histogram manager
};

#endif // DATAANALYZER_H

