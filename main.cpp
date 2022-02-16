#include <QApplication>

#include "mainwindow.h"
#include "myapp.h"

void loadLocal();
extern QVector<QString> filelist;
QWidgetList wdlist;
extern QString PlistFileName;
extern bool zh_cn;
MainWindow *mw_one;

int main(int argc, char *argv[]) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
  {
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  }
#endif

  MyApplication *a = new MyApplication(argc, argv);
  loadLocal();
  QString strAppExePath = qApp->applicationDirPath();
  mw_one->copyDirectoryFiles(strAppExePath + "/Database/",
                             QDir::homePath() + "/Database/", false);
  QString fileSample =
      QDir::homePath() + "/Database/BaseConfigs/SampleCustom.plist";
  if (!QFile(fileSample).exists()) {
    QMessageBox::critical(
        NULL, "",
        fileSample + "\n\n" +
            QObject::tr("The file does not exist, please check the "
                        "integrity of the app."));

    return 0;
  }

  QFont f;
#ifdef Q_OS_WIN32

  PlistFileName = QString::fromLocal8Bit(argv[1]);  //解决乱码

  f.setFamily("Microsoft YaHei UI");

#endif

#ifdef Q_OS_MAC

#endif

  if (!PlistFileName.isEmpty()) {
    a->new_win();
  } else {
    mw_one = new MainWindow();
    mw_one->show();
  }

  f.setPixelSize(12);
  a->setFont(f);
  return a->exec();
}

void loadLocal() {
  static QTranslator translator;
  static QTranslator translator1;
  static QTranslator translator2;

  QLocale locale;
  if (locale.language() == QLocale::English) {
    zh_cn = false;

  } else if (locale.language() == QLocale::Chinese) {
    bool tr = false;
    tr = translator.load(":/cn.qm");
    if (tr) {
      qApp->installTranslator(&translator);
      zh_cn = true;
    }

    bool tr1 = false;
    tr1 = translator1.load(":/qt_zh_CN.qm");
    if (tr1) {
      qApp->installTranslator(&translator1);
      zh_cn = true;
    }

    bool tr2 = false;
    tr2 = translator2.load(":/widgets_zh_cn.qm");
    if (tr2) {
      qApp->installTranslator(&translator2);
      zh_cn = true;
    }

    // ui->retranslateUi(this);
  }
}
