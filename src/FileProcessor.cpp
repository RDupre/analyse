#include "FileProcessor.h"
#include <iostream>
#include <cmath>

FileProcessor::FileProcessor(const std::vector<std::string>& filenames, HistogramManager& histManager)
    : filenames(filenames), histManager(histManager) {}

void FileProcessor::processFiles() {
    for (const auto& filename : filenames) {
        std::cout << "Processing file: " << filename << std::endl;
        processFile(filename);
    }
}

void FileProcessor::processFile(const std::string& filename) {
    hipo::reader reader;
    reader.open(filename.c_str());

    hipo::dictionary factory;
    reader.readDictionary(factory);

    hipo::bank RUNconfig = factory.getSchema("RUN::config");
    hipo::bank RECevt = factory.getSchema("REC::Event");
    hipo::bank RECpart = factory.getSchema("REC::Particle");
    hipo::bank ALEtrk = factory.getSchema("AHDC::Track");

    hipo::event evt;
    int eventCount = 0;

    while (reader.next()) {
        reader.read(evt);
        evt.getStructure(RUNconfig);
        evt.getStructure(RECpart);
        evt.getStructure(RECevt);
        evt.getStructure(ALEtrk);

        histManager.fillHistograms(RECpart, ALEtrk);
    }
}
