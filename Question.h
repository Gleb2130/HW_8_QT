#pragma once
#include <QString>
#include <QVector>


class Question
{
    QString text;
    QVector<QString> options;
    int correctIndex;
public:
    Question(const QString& text = "", const QVector<QString>& options = {}, int correctIndex = 0)
        : text(text), options(options), correctIndex(correctIndex) {};

    QString getText() const { return text; }
    QVector<QString> getOptions() const { return options; }
    int getCorrectIndex() const { return correctIndex; }
};

