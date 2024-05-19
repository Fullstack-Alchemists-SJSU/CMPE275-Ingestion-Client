#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>

class CSVProcessor
{
public:
    explicit CSVProcessor(const QString &directory);
    QVector<QJsonDocument> processFiles();

private:
    QString directory;
    QVector<QString> lines;

    void loadFile(const QString &filePath);
    void removeInvalidEntries();
    void removeDuplicates();
    void convertDateTime();
    QJsonObject serializeDataToJson();
};

#endif // CSVPROCESSOR_H
