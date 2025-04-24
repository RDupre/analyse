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
#include "FileListReader.h"

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

    // Use FileListReader to handle the input file
    FileListReader fileListReader(argv[1]);
    if (!fileListReader.isValid()) {
        return 1;
    }

    const auto& filenames = fileListReader.getFilenames();

    std::cout << "Start processing files..." << std::endl;

    // Initialize HistogramManager and FileProcessor
    DataAnalyzer MyDataAnalyzer;
    FileProcessor fileProcessor(filenames, MyDataAnalyzer);

    // Process files
    fileProcessor.processFiles();

    std::cout << "Processing complete." << std::endl;
    return 0;
}
