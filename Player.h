#pragma once
#include <QString>

class Player
{
	QString name;
	double score;
	int jokers;
public:
	Player(const QString& name = "") :name(name), score(0.), jokers(0) {};

	void setName(const QString& newName) { name = newName; }
	QString getName() const { return name; }

	void setScore(double newScore) { score = newScore; }
	double getScore() const { return score; }

	void setJokers(int newJokers) { jokers = newJokers; }
	int getJokers() const { return jokers; }
};

