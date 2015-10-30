#include "hadith.h"

Hadith::Hadith()
{

}

Hadith::Hadith(const QString &hadith, const QString &arawi)
{
    this->hadith = hadith;
    this->arawi = arawi;
}

QString Hadith::getArawi() const
{
    return arawi;
}

void Hadith::setArawi(const QString &arawi)
{
    this->arawi = arawi;
}

QString Hadith::getHadith() const
{
    return hadith;
}

void Hadith::setHadith(const QString &hadith)
{
    this->hadith = hadith;
}

int Hadith::getNumberOfWordsInAHadith() const
{
    return hadith.count(" ") + 1;
}

QString Hadith::getUntraitedText() const
{
    return untraitedText;
}

void Hadith::setUntraitedText(const QString &untraitedText)
{
    this->untraitedText = untraitedText;
}

QString Hadith::getCurrentWord() const
{
    return currentWord;
}

void Hadith::setCurrentWord(const QString &currentWord)
{
    this->currentWord = currentWord;
}

QString Hadith::getTraitedText() const
{
    return traitedText;
}

void Hadith::setTraitedText(const QString &traitedText)
{
    this->traitedText = traitedText;
}


int Hadith::calculateTimeForTheHadith() const
{
    // 95 SEC -> 34 WORD
    // X  SEC -> HADITH WORDS
    return this->getNumberOfWordsInAHadith() * 95 / 34;
}



