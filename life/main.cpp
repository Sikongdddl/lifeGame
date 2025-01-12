#include "widget.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Widget w;
    w.resize(500,500);
    w.setWindowTitle("Life game");
    w.show();

    return a.exec();
}
