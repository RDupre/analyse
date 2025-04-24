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
    HistogramManager histManager0;
    HistogramManager histManager1;
    HistogramManager histManager2;
};

#endif // DATAANALYZER_H

