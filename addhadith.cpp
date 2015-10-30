#include "addhadith.h"
#include "ui_addhadith.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>

AddHadith::AddHadith(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddHadith)
{
    ui->setupUi(this);

    this->parent = (Widget*) parent;

    // FILL THE LIST OF THE ROWAT
    QStringList rowat;
    ui->rawiComboBox->addItem("البخاري", "البخاري");
    ui->rawiComboBox->addItem("مسلم", "مسلم");
    ui->rawiComboBox->addItem("النسائي", "النسائي");
    ui->rawiComboBox->addItem("أبو داود", "أبو داود");
    ui->rawiComboBox->addItem("إبن ماجه", "إبن ماجه");
    ui->rawiComboBox->addItem("الترميذي", "الترميذي");
    ui->rawiComboBox->addItem("أحمد بن حنبل", "أحمد بن حنبل");
    ui->rawiComboBox->addItem("الألباني", "الألباني");
    ui->rawiComboBox->addItem("البيهقي", "البيهقي");
    ui->rawiComboBox->addItem("متفق عليه", "متفق عليه");
    

    // CONNECT TO THE DATABASE
    // SETUP THE AHADITH DATABASE
    QString dirQString = "/.kotab/files/";
    QDir dir(QDir::homePath() + dirQString);
    QString databasePath = dir.absolutePath() + "/" + "ahadith.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    if(!QFile(databasePath).exists()) {
        db.open();
        QSqlQuery query;
        query.exec("CREATE TABLE hadith "
                  "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "rawi VARCHAR(50), "
                  "hadith TEXT)");
        db.close();
    }

    connect(ui->addHadithButton, SIGNAL(clicked(bool)), this, SLOT(addHadith(bool)));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(closeDialog(bool)));
}

void AddHadith::addHadith(bool)
{
    Hadith hadith;
    hadith.setArawi(ui->rawiComboBox->itemData(ui->rawiComboBox->currentIndex()).toString());
    qDebug() << ui->rawiComboBox->itemData(0).toString();
    hadith.setHadith(ui->hadithTextEdit->toPlainText());
    // ADD HADITH TO DATABASE
    db.open();
    QSqlQuery query;
    if(query.exec(QString("INSERT INTO hadith (rawi, hadith) VALUES ('%1','%2')").arg(hadith.getArawi(), hadith.getHadith()))) {
        QMessageBox::information(this,
                                 "إضافة حديث",
                                 "لقد تم إضافة الحديث بنجاح، قم من فضلك بإعادة تشغيل البرنامج لتفعيل التغييرات.");
    } else {
        QMessageBox::critical(this,
                                 "إضافة حديث",
                                 "للأسف لم نستطع إضافة الحديث بسبب مشكل في قواعد البيانات.");
    }
    db.close();
    closeDialog(true);
}

void AddHadith::closeDialog(bool)
{
    close();
}

AddHadith::~AddHadith()
{
    delete ui;
}
