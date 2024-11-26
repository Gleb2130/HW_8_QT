#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HW_8_QT.h"
#include "Player.h"
#include "Question.h"
#include <vector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <random>

class HW_8_QT : public QMainWindow
{
    Q_OBJECT

public:
    HW_8_QT(QWidget* parent = nullptr);
    ~HW_8_QT();

private:
    Ui::HW_8_QTClass ui;

    Player player;
    std::vector<Question> questions;
    int currentQuestionIndex = -1;
    bool isRandomOrder = false;
    std::vector<int> questionOrder;

    void loadQuestions();;
    void displayQuestion(bool useJoker);
    void resetUI();

private slots:
    void startNewGame();
    void endGame();
    void confirmAnswer();
    void skipQuestion();
    void useJoker();
};
