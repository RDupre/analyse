#include "FileProcessor.h"
#include <iostream>
#include <cmath>
#include <regex>

FileProcessor::FileProcessor(const std::vector<std::string>& filenames, HistogramManager& histManager)
    : filenames(filenames), histManager(histManager) {}

void FileProcessor::processFiles() {
    for (const auto& filename : filenames) {
        if (!isValidHipoFile(filename)) {
            // Extract the last 5 digits from the filename
            std::regex digitRegex("\\d{5}$");
            std::smatch match;
            if (std::regex_search(filename, match, digitRegex)) {
                int RunNumber = std::stoi(match.str());
                std::cout << "Detected signal file: " << filename << " with RunNumber: " << RunNumber << std::endl;
                histManager.handleRunNumber(RunNumber);
            } else {
                std::cerr << "Skipping invalid file: " << filename << " (not a .hipo file and no valid RunNumber)" << std::endl;
            }
            continue;
        }
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

bool FileProcessor::isValidHipoFile(const std::string& filename) const {
    const std::string extension = ".hipo";
    if (filename.size() >= extension.size() &&
        filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0) {
        return true;
    }
    return false;
}
