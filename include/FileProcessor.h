#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <vector>
#include "hipo4/reader.h"
#include "HistogramManager.h"

class FileProcessor {
public:
    FileProcessor(const std::vector<std::string>& filenames, HistogramManager& histManager);
    void processFiles();

private:
    std::vector<std::string> filenames;
    HistogramManager& histManager;
    void processFile(const std::string& filename);
};

#endif // FILEPROCESSOR_H