#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <vector>
#include <string>
#include "DataAnalyzer.h"
#include "hipo4/reader.h"

class FileProcessor {
public:
    FileProcessor(const std::vector<std::string>& filenames, DataAnalyzer& dataAnalyzer);
    void processFiles();

private:
    std::vector<std::string> filenames;
    DataAnalyzer& dataAnalyzer;

    void processFile(const std::string& filename);
    bool isValidHipoFile(const std::string& filename) const;
};

#endif // FILEPROCESSOR_H