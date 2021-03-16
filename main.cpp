#include "mainwindow.h"
#include "myapp.h"
#include <QApplication>

extern QVector<QString> filelist;
extern QWidgetList wdlist;
extern QString PlistFileName;
MainWindow* mw_one;

int main(int argc, char* argv[])
{

#ifdef Q_OS_LINUX
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

#endif

    // QApplication a(argc, argv);
    MyApplication* a = new MyApplication(argc, argv);

#ifdef Q_OS_WIN32

    PlistFileName = QString::fromLocal8Bit(argv[1]); //解决乱码

    QFont f;
    f.setFamily("Microsoft YaHei UI");
    a->setFont(f);
#endif

#ifdef Q_OS_MAC

#endif

    if (!PlistFileName.isEmpty()) {
        a->new_win();
    }

    else {

        mw_one = new MainWindow();
        mw_one->show();
    }

    return a->exec();
}
