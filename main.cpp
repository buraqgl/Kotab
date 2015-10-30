#include "widget.h"
#include "addhadith.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Widget w;
    w.show();

    return a.exec();
}
