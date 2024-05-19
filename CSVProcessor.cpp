#include "CSVProcessor.h"
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <omp.h>
#include <iomanip>
#include <ctime>

using namespace std;

CSVProcessor::CSVProcessor(const std::string &inputFile, const std::string &outputFile)
    : inputFile(inputFile), outputFile(outputFile) {}

void CSVProcessor::processFile()
{
    loadFile();
    removeInvalidEntries();
    removeDuplicates();
    convertDateTime();
    saveToFile();
}

void CSVProcessor::loadFile()
{
    ifstream file(inputFile);
    string line;
    if (!file.is_open())
    {
        cerr << "Error opening file: " << inputFile << endl;
        return;
    }
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();
    cout << "Total lines read: " << lines.size() << endl;
}

void CSVProcessor::removeInvalidEntries()
{
    vector<string> validLines;
#pragma omp parallel
    {
        vector<string> privateValidLines;
#pragma omp for nowait
        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].find("-999") == string::npos)
            {
                privateValidLines.push_back(lines[i]);
            }
        }
#pragma omp critical
        validLines.insert(validLines.end(), privateValidLines.begin(), privateValidLines.end());
    }
    lines.swap(validLines);
    cout << "Lines after removing invalid entries: " << lines.size() << endl;
}

void CSVProcessor::removeDuplicates()
{
    set<string> seen;
    vector<string> uniqueLines;

#pragma omp parallel
    {
        set<string> privateSeen;
        vector<string> privateUniqueLines;

#pragma omp for nowait
        for (int i = 0; i < lines.size(); ++i)
        {
            if (privateSeen.insert(lines[i]).second)
            {
                privateUniqueLines.push_back(lines[i]);
            }
        }

#pragma omp critical
        {
            for (const auto &line : privateUniqueLines)
            {
                if (seen.insert(line).second)
                {
                    uniqueLines.push_back(line);
                }
            }
        }
    }
    lines.swap(uniqueLines);
    cout << "Lines after removing duplicate entries: " << lines.size() << endl;
}

// Function to remove quotes from a string
std::string removeQuotes(std::string input)
{
    input.erase(std::remove(input.begin(), input.end(), '"'), input.end());
    return input;
}

void CSVProcessor::convertDateTime()
{
    stringstream ss;
    string item, newLine;
    tm t = {};
    char buffer[20];

#pragma omp parallel for private(ss, item, newLine, t, buffer) schedule(dynamic)
    for (int i = 0; i < lines.size(); i++)
    {
        ss.clear();
        ss.str(lines[i]);

        getline(ss, item, ',');
        newLine = item + ","; // Discard the first item

        getline(ss, item, ',');
        newLine += item + ","; // Discard the second item

        if (getline(ss, item, ','))
        {
            std::string cleanedDate = removeQuotes(item);
            istringstream dateStream(cleanedDate);
            dateStream >> get_time(&t, "%Y-%m-%dT%H:%M"); // ISO 8601 format

            if (dateStream.fail())
            {
                cerr << "Date parsing failed for: " << item << " on line " << i << endl;
                dateStream.clear(); // Clear error state
                continue;           // Skip processing this line or handle it appropriately
            }

            strftime(buffer, 20, "%Y-%m-%d %H:%M", &t);
            newLine += string(buffer);
        }

        // Append the rest of the line
        while (getline(ss, item, ','))
        {
            newLine += "," + item;
        }

// Critical section may be needed if we write to shared data
#pragma omp critical
        lines[i] = newLine; // Update the original line with the new formatted line
    }
    cout << "Lines after date conversion: " << lines.size() << endl;
}

void CSVProcessor::saveToFile()
{
    ofstream out(outputFile, ios::trunc);
    if (!out.is_open())
    {
        cerr << "Failed to open output file: " << outputFile << endl;
        return;
    }
    for (const auto &line : lines)
    {
        out << line << endl;
    }
    out.close();
    cout << "Data written to output file." << endl;
}
