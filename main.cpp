#include "mainwindow.h"
#include "singleapplication.h"
#include "common.h"
#include "darkfusionstyle.h"
#include "commodorestyle.h"
#include "fabricstyle.h"
#include "customstyle.h"
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    SingleApplication a(argc, argv);
    if(argc == 0)
    {
        if(!a.lock()) return Common::ALREADYRUNNING;
    }
    a.setWindowIcon(QIcon(":/res/icon/ide65xx.png"));

    // add font
    QFontDatabase::addApplicationFont(":/res/font/UbuntuMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/res/font/UbuntuMono-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/res/font/UbuntuMono-Italic.ttf");
    QFontDatabase::addApplicationFont(":/res/font/UbuntuMono-Regular.ttf");

    QSettings settings(Common::appConfigDir()+"/settings.ini", QSettings::IniFormat);

    // application style
    int style = settings.value("ApplicationStyle", 3).toInt();
    switch (style) {
        case 0: qApp->setStyle(QStyleFactory::create("fusion")); break;     // fusion
        case 1: qApp->setStyle(new DarkFusionStyle()); break;               // darkfusion
        case 2: qApp->setStyle(QStyleFactory::create("windows")); break;    // windows
        case 3: qApp->setStyle(new CommodoreStyle()); break;                // commodore
        case 4: qApp->setStyle(new FabricStyle()); break;                   // fabric
        case 5: qApp->setStyle(new CustomStyle()); break;                   // custom
    }

    qApp->setPalette(QApplication::style()->standardPalette());

    // tooltip style
    qApp->setStyleSheet("QToolTip { color: #000000; background-color: #f8f8f8; border: 1px solid #b0b0b0; }");

    // font
    QString fontFamily = settings.value("ApplicationFontName", "Ubuntu Mono").toString();
    int fontSize = settings.value("ApplicationFontSize", 9).toInt();
    a.setFont(QFont(fontFamily, fontSize));

    MainWindow w;
    w.show();

    return a.exec();
}
