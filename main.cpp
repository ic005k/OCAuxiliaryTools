#include "mainwindow.h"
#include "myapp.h"
#include <QApplication>

extern QVector<QString> filelist;
extern QWidgetList wdlist;
extern QString PlistFileName;
MainWindow* mw_one;

int main(int argc, char* argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    {
        qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    }
#endif

    // QApplication a(argc, argv);
    MyApplication* a = new MyApplication(argc, argv);

    QFont f;
#ifdef Q_OS_WIN32

    PlistFileName = QString::fromLocal8Bit(argv[1]); //解决乱码

    f.setFamily("Microsoft YaHei UI");

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

    f.setPixelSize(12);
    a->setFont(f);
    return a->exec();
}
