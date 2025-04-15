#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <optional>
#include <sstream>

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

#include "hipo4/reader.h"
#include "hipo4/bank.h"
#include "hipo4/dictionary.h"
#include "FileProcessor.h"
#include "HistogramManager.h"

int main(int argc, char* argv[]) {
    std::ostringstream command;
    for (int i = 0; i < argc; ++i) {
        command << argv[i];
        if (i < argc - 1) {
            command << " ";
        }
    }
    std::cout << "Command: " << command.str() << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_list.txt>" << std::endl;
        return 1;
    }

    // Read the list of files from the provided file
    std::string fileListPath = argv[1];
    std::ifstream fileList(fileListPath);
    if (!fileList.is_open()) {
        std::cerr << "Error: Could not open file list " << fileListPath << std::endl;
        return 1;
    }

    std::vector<std::string> filenames;
    std::string line;
    while (std::getline(fileList, line)) {
        if (!line.empty()) {
            filenames.push_back(line);
        }
    }
    fileList.close();

    if (filenames.empty()) {
        std::cerr << "Error: No files found in the list." << std::endl;
        return 1;
    }

    std::cout << "Start processing files..." << std::endl;

    // Initialize HistogramManager and FileProcessor
    HistogramManager histManager;
    FileProcessor fileProcessor(filenames, histManager);

    // Process files
    fileProcessor.processFiles();

    // Write histograms
    histManager.writeHistograms("output/output.root");

    std::cout << "Processing complete." << std::endl;
    return 0;
}
