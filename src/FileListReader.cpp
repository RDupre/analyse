#include "FileListReader.h"

FileListReader::FileListReader(const std::string& filePath) : filePath(filePath), valid(false) {
    readFile();
}

void FileListReader::readFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file list " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            filenames.push_back(line);
        }
    }
    file.close();

    if (filenames.empty()) {
        std::cerr << "Error: No files found in the list." << std::endl;
    } else {
        valid = true;
    }
}

bool FileListReader::isValid() const {
    return valid;
}

const std::vector<std::string>& FileListReader::getFilenames() const {
    return filenames;
}