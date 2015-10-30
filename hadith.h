#ifndef HADITH_H
#define HADITH_H

#include <QString>

class Hadith
{
public:
    Hadith();
    Hadith(const QString&, const QString&);
    QString getHadith() const;
    void setHadith(const QString&);
    QString getArawi() const;
    void setArawi(const QString&);
    int calculateTimeForTheHadith() const;
    QString getTraitedText() const;
    void setTraitedText(const QString &);
    QString getCurrentWord() const;
    void setCurrentWord(const QString &);
    QString getUntraitedText() const;
    void setUntraitedText(const QString &);
    int getNumberOfWordsInAHadith() const;

private:
    QString hadith;
    QString arawi;
    QString traitedText;
    QString currentWord;
    QString untraitedText;
};

#endif // HADITH_H
