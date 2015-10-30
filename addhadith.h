#ifndef ADDHADITH_H
#define ADDHADITH_H

#include "hadith.h"
#include "widget.h"

#include <QDialog>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

namespace Ui {
class AddHadith;
}

class AddHadith : public QDialog
{
    Q_OBJECT

public:
    explicit AddHadith(QWidget *parent = 0);
    ~AddHadith();

private slots:
    void addHadith(bool);
    void closeDialog(bool);

private:
    Ui::AddHadith *ui;
    QSqlDatabase db;
    Widget* parent;
};

#endif // ADDHADITH_H
