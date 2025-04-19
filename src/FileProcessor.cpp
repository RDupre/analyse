#include "FileProcessor.h"
#include "DataAnalyzer.h" // Updated include
#include <iostream>
#include <cmath>
#include <regex>

FileProcessor::FileProcessor(const std::vector<std::string>& filenames, DataAnalyzer& dataAnalyzer)
    : filenames(filenames), dataAnalyzer(dataAnalyzer) {}

void FileProcessor::processFiles() {
    for (const auto& filename : filenames) {
        if (!isValidHipoFile(filename)) {
            // Extract the last 5 digits from the filename
            std::regex digitRegex("\\d{5}$");
            std::smatch match;
            if (std::regex_search(filename, match, digitRegex)) {
                int RunNumber = std::stoi(match.str());
                std::cout << "Detected signal file: " << filename << " with RunNumber: " << RunNumber << std::endl;
                dataAnalyzer.handleRunNumber(RunNumber);
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

    hipo::bank RECpart = factory.getSchema("REC::Particle");
    hipo::bank ALEtrk = factory.getSchema("AHDC::track");
    hipo::bank ALEadc = factory.getSchema("AHDC::adc");
    hipo::bank ALEhit = factory.getSchema("AHDC::hits");

    hipo::event evt;

    while (reader.next()) {
        reader.read(evt);
        evt.getStructure(RECpart);
        evt.getStructure(ALEtrk );
        evt.getStructure(ALEadc );
        evt.getStructure(ALEhit );

        dataAnalyzer.analyzeEvent(RECpart, ALEtrk, ALEadc, ALEhit);
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
