#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include "hipo4/bank.h"
#include "HistogramManager.h"

class DataAnalyzer {
public:
    explicit DataAnalyzer(HistogramManager& histManager);
    void analyzeEvent(hipo::bank& RECpart, hipo::bank& ALEtrk);

private:
    HistogramManager& histManager;
};

#endif // DATAANALYZER_H