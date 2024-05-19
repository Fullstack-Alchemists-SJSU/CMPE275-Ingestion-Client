#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <string>
#include <vector>

class CSVProcessor
{
public:
    CSVProcessor(const std::string &inputFile, const std::string &outputFile);
    void processFile();

private:
    std::string inputFile;
    std::string outputFile;
    std::vector<std::string> lines;

    void loadFile();
    void removeInvalidEntries();
    void removeDuplicates();
    void convertDateTime();
    void saveToFile();
};

#endif // CSVPROCESSOR_H
