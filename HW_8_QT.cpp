#include "HW_8_QT.h"
#include <QMessageBox>
#include <algorithm>

using namespace std;

HW_8_QT::HW_8_QT(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.difficultyComboBox->addItem("Ľahká");
    ui.difficultyComboBox->addItem("Stredná");
    ui.difficultyComboBox->addItem("Zložitá");


    connect(ui.newGameButton, &QPushButton::clicked, this, &HW_8_QT::startNewGame);
    connect(ui.endGameButton, &QPushButton::clicked, this, &HW_8_QT::endGame);
    connect(ui.confirmButton, &QPushButton::clicked, this, &HW_8_QT::confirmAnswer);
    connect(ui.skipButton, &QPushButton::clicked, this, &HW_8_QT::skipQuestion);
    connect(ui.jokerButton1, &QPushButton::clicked, this, &HW_8_QT::useJoker);
    connect(ui.jokerButton2, &QPushButton::clicked, this, &HW_8_QT::useJoker);
    connect(ui.jokerButton3, &QPushButton::clicked, this, &HW_8_QT::useJoker);

    loadQuestions();
    resetUI();
}

HW_8_QT::~HW_8_QT()
{}

void HW_8_QT::loadQuestions() {
    QFile file("questions.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "ERROR", "Nemožno otvoriť súbor questions.txt!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(';');
        if (parts.size() == 6) { 
            Question question(parts[0], { parts[1], parts[2], parts[3], parts[4] }, parts[5].toInt());
            questions.push_back(question);
        }
    }
    file.close();

    if (questions.empty()) {
        QMessageBox::critical(this, "ERROR", "V súbore sa nenašli žiadne otázky!");
    }
}




void HW_8_QT::resetUI() {
    ui.nameLineEdit->clear();
    ui.scoreSpinBox->setValue(0.0);
    ui.scoreSpinBox->setReadOnly(true); 
    ui.difficultyComboBox->setCurrentIndex(0);
    ui.randomOrderCheckBox->setChecked(false);
    ui.questionTextEdit->clear();
    ui.answerARadioButton->setChecked(false);
    ui.answerBRadioButton->setChecked(false);
    ui.answerCRadioButton->setChecked(false);
    ui.answerDRadioButton->setChecked(false);
    ui.jokerButton1->setEnabled(false);
    ui.jokerButton2->setEnabled(false);
    ui.jokerButton3->setEnabled(false);
}


void HW_8_QT::displayQuestion(bool useJoker = 0)
{ 
    if (currentQuestionIndex >= questionOrder.size() || questionOrder.empty()) {
        QMessageBox::information(this, "Koniec", QString("Skóre: %1").arg(player.getScore()));
        resetUI();
        return;
    }

    if (!useJoker) {
        ui.answerARadioButton->setVisible(true);
        ui.answerBRadioButton->setVisible(true);
        ui.answerCRadioButton->setVisible(true);
        ui.answerDRadioButton->setVisible(true);
    }

    int qIndex = questionOrder[currentQuestionIndex];
    const Question& question = questions[qIndex];

    ui.questionTextEdit->setPlainText(question.getText());
    ui.answerARadioButton->setText(question.getOptions()[0]);
    ui.answerBRadioButton->setText(question.getOptions()[1]);
    ui.answerCRadioButton->setText(question.getOptions()[2]);
    ui.answerDRadioButton->setText(question.getOptions()[3]);

    ui.jokerButton1->setEnabled(player.getJokers() > 0);
    ui.jokerButton2->setEnabled(player.getJokers() > 1);
    ui.jokerButton3->setEnabled(player.getJokers() > 2);
}





void HW_8_QT::confirmAnswer() {
    int selectedAnswer = ui.answerARadioButton->isChecked() ? 0 :
        ui.answerBRadioButton->isChecked() ? 1 :
        ui.answerCRadioButton->isChecked() ? 2 :
        ui.answerDRadioButton->isChecked() ? 3 : -1;

    if (selectedAnswer == -1) {
        QMessageBox::warning(this, "ERROR", "Vyberte si odpoveď!");
        return;
    }


    int qIndex = questionOrder[currentQuestionIndex];
    if (selectedAnswer == questions[qIndex].getCorrectIndex()) {
        player.setScore(player.getScore() + 1);
    }
    else {
        player.setScore(player.getScore() - 1);
    }


    currentQuestionIndex++;
    displayQuestion();
}


void HW_8_QT::skipQuestion() {
    player.setScore(player.getScore() - 0.5);

    currentQuestionIndex++;
    displayQuestion();
}


void HW_8_QT::useJoker() {
    if (player.getJokers() <= 0) return;

    player.setJokers(player.getJokers() - 1);
    int qIndex = questionOrder[currentQuestionIndex];
    int correctAnswer = questions[qIndex].getCorrectIndex();

    if (sender() == ui.jokerButton1 || sender() == ui.jokerButton2 || sender() == ui.jokerButton3) {

        vector<int> wrongAnswers;
        for (int i = 0; i < 4; ++i) {
            if (i != correctAnswer) {
                wrongAnswers.push_back(i);
            }
        }

        int randomWrongAnswer = wrongAnswers[rand() % wrongAnswers.size()];

        int index = 0;

        if (rand() % 2 == 0) {
            index = randomWrongAnswer;
        }
        else {
            index = correctAnswer;
        }

        ui.answerARadioButton->setChecked(index==0);
        ui.answerBRadioButton->setChecked(index==1);
        ui.answerCRadioButton->setChecked(index==2);
        ui.answerDRadioButton->setChecked(index==3);

        
        (randomWrongAnswer == 0 || correctAnswer == 0) ? ui.answerARadioButton->setVisible(true):ui.answerARadioButton->setVisible(false);
        (randomWrongAnswer == 1 || correctAnswer == 1) ? ui.answerBRadioButton->setVisible(true):ui.answerBRadioButton->setVisible(false);
        (randomWrongAnswer == 2 || correctAnswer == 2) ? ui.answerCRadioButton->setVisible(true):ui.answerCRadioButton->setVisible(false);
        (randomWrongAnswer == 3 || correctAnswer == 3) ? ui.answerDRadioButton->setVisible(true):ui.answerDRadioButton->setVisible(false);

    }

    displayQuestion(true);
}



void HW_8_QT::startNewGame() {
    QString name = ui.nameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "ERROR", "Zadajte meno!");
        return;
    }

    player.setName(name);
    player.setScore(0.0);

    int difficultyIndex = ui.difficultyComboBox->currentIndex();
    if (difficultyIndex == 0) {
        player.setJokers(3);
    }
    else if (difficultyIndex == 1) {
        player.setJokers(2);
    }
    else {
        player.setJokers(1);
    }

    currentQuestionIndex = 0;
    isRandomOrder = ui.randomOrderCheckBox->isChecked();

    if (questions.empty()) {
        QMessageBox::critical(this, "ERROR", "Bez otázok!");
        return;
    }

    questionOrder.resize(questions.size());
    iota(questionOrder.begin(), questionOrder.end(), 0);
    if (isRandomOrder) {
        srand(time(0));

        for (size_t i = questionOrder.size() - 1; i > 0; --i) {
            size_t j = rand() % (i + 1); 
            swap(questionOrder[i], questionOrder[j]);
        }
        shuffle(questionOrder.begin(), questionOrder.end(), mt19937{ random_device{}() });
    }

    if (questionOrder.empty()) {
        QMessageBox::critical(this, "ERROR", "V hre nie sú k dispozícii žiadne otázky!");
        return;
    }

    displayQuestion();
}



void HW_8_QT::endGame() {
    QMessageBox::information(this, "Koniec", QString("Skóre: %1").arg(player.getScore()));
    resetUI();
}