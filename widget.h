#ifndef WIDGET_H
#define WIDGET_H

#include "hadith.h"

#include <QWidget>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setup();

private slots:
    void timeOutSlot();
    void wordChanged(QString);
    void trainAgainButtonClicked(bool);
    void openAddHadith(bool);   

private:
    Ui::Widget *ui;
    QList<Hadith> ahadithList;
    Hadith currentHadith;
    int generateRandom(const int&, const int&);
    int currentSec;
    int totalTime;
    QTimer *timer;
    void setupText(const int &numberOfWordsTraited);
    int currentPosition;
    QString pinkHtml;
    QString greenHtml;
    QString blackHtml;
    QString endHtml;
    int calculateSpeed() const;
    bool startTheCounter;
    void showResult(const QString&);
    int currentSpeed;
    QString readRecordFromFile();
    void writeRecordToFile(const int&);
    int record;
    bool isDone;
    QFile *recordFile;
    QSqlDatabase db;
    void readDataFromDatabase();
    void addHadith(const Hadith&);
};

#endif // WIDGET_H
