#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // set the app and org
    QCoreApplication::setOrganizationName("System");
    QCoreApplication::setOrganizationDomain("system-group.it");
    QCoreApplication::setApplicationName("Statistic Ripper");

    QSettings *settings = new QSettings("System", "Statistic Ripper");

    QString language;

    if(settings->value("settingsDialog/language").toString() == "System language")
        language = QString("%1").arg(QLocale::system().name());
    else if(settings->value("settingsDialog/language").toString() == "Italian")
        language = "it_IT";
    else if(settings->value("settingsDialog/language").toString() == "English")
        language = "en_US";


    delete settings;
    QTranslator translator;
    translator.load(QString(":/translations/sr_") + language);
    app.installTranslator(&translator);

    MainWindow w;
    w.show();
    return app.exec();
}

