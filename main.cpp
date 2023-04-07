#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ISAR_IMAGING);

    QApplication a(argc, argv);

    // set app information
    QCoreApplication::setApplicationName(QObject::tr("ISAR Imaging"));
    QCoreApplication::setOrganizationName(QObject::tr("SYSU"));
    QCoreApplication::setOrganizationDomain(QString("https://www.sysu.edu.cn/index.htm"));
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    a.setWindowIcon(QIcon(":/img/resources/LOGO.png"));

    // set up translator
    QTranslator translator;
    const QString uiLanguages = "zh_CN"; // en_US
    if (translator.load(":language/resources/language/" + uiLanguages + ".qm")) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
