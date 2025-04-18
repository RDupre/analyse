#ifndef FILELISTREADER_H
#define FILELISTREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class FileListReader {
public:
    explicit FileListReader(const std::string& filePath);
    bool isValid() const;
    const std::vector<std::string>& getFilenames() const;

private:
    std::string filePath;
    std::vector<std::string> filenames;
    bool valid;

    void readFile();
};

#endif // FILELISTREADER_H