#include "CSVProcessor.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

CSVProcessor::CSVProcessor(const QString &directory) : directory(directory) {}

QVector<QJsonDocument> CSVProcessor::processFiles()
{
    QDir dir(directory);
    QStringList csvFiles = dir.entryList(QStringList() << "*.csv", QDir::Files);
    QVector<QJsonDocument> documents;

    foreach (const QString &file, csvFiles)
    {
        QString filePath = dir.filePath(file);
        loadFile(filePath);
        removeInvalidEntries();
        removeDuplicates();
        convertDateTime();
        QJsonObject json = serializeDataToJson();
        documents.append(QJsonDocument(json));
    }
    return documents;
}

void CSVProcessor::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error opening file:" << filePath;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        lines.append(in.readLine());
    }
    file.close();
}

void CSVProcessor::removeInvalidEntries()
{
    QVector<QString> validLines;
    validLines.reserve(lines.size());
#pragma omp parallel
    {
        QVector<QString> privateValidLines;
#pragma omp for nowait
        for (int i = 0; i < lines.size(); ++i)
        {
            if (!lines[i].contains("-999"))
            {
                privateValidLines.append(lines[i]);
            }
        }
#pragma omp critical
        validLines.append(privateValidLines);
    }
    lines = validLines;
    qDebug() << "Lines after removing invalid entries: " << lines.size();
}

void CSVProcessor::removeDuplicates()
{
    QSet<QString> seen;
    QVector<QString> uniqueLines;
    uniqueLines.reserve(lines.size());
#pragma omp parallel
    {
        QSet<QString> privateSeen;
        QVector<QString> privateUniqueLines;
#pragma omp for nowait
        for (int i = 0; i < lines.size(); ++i)
        {
            QString line = lines[i];
#pragma omp critical
            {
                if (seen.insert(line).second)
                {
                    privateUniqueLines.append(line);
                }
            }
        }
#pragma omp critical
        uniqueLines.append(privateUniqueLines);
    }
    lines = uniqueLines;
    qDebug() << "Lines after removing duplicates: " << lines.size();
}

// Function to remove quotes from a string
std::string removeQuotes(std::string input)
{
    input.erase(std::remove(input.begin(), input.end(), '"'), input.end());
    return input;
}

void CSVProcessor::convertDateTime()
{
    int n = lines.size();
    QVector<QString> newLines(n); // Preallocate vector to avoid resizing inside the loop

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; ++i)
    {
        QTextStream ss(&lines[i]);
        QString newLine;
        QString item;

        // Read the first item which is assumed to be the date-time column
        ss >> item;

        // Append the unique identifier "@<row_number+1>"
        newLine = item + "@" + QString::number(i + 1);

        // Read and append the rest of the line
        while (!ss.atEnd())
        {
            ss >> item;
            newLine += "," + item;
        }

        // Update the line in the newLines vector
        newLines[i] = newLine;
    }

    // Swap the old lines with the newly processed ones
    lines = newLines;
    qDebug() << "Lines after adding unique IDs: " << lines.size();
}

QJsonObject CSVProcessor::serializeDataToJson()
{
    QJsonObject obj;
    QJsonArray dataArray;
    for (const QString &line : lines)
    {
        QJsonObject lineObj; // Assume each line is converted into a JSON object
        dataArray.append(lineObj);
    }
    obj["Data"] = dataArray;
    return obj;
}