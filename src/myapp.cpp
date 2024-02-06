#include "myapp.h"

#include <QSplashScreen>

#include "mainwindow.h"

extern QString PlistFileName;
extern QVector<QString> openFileLists;
extern QWidgetList wdlist;
extern MainWindow* mw_one;

bool MyApplication::event(QEvent* event) {
  if (event->type() == QEvent::FileOpen) {
    QFileOpenEvent* openEvent = static_cast<QFileOpenEvent*>(event);
    PlistFileName = openEvent->file();

    if (mw_one->isActiveWindow()) mw_one->openFile(PlistFileName);
  }

  return QApplication::event(event);
}

void MyApplication::new_win() {
  if (!PlistFileName.isEmpty()) {
    bool newfile = true;
    for (int i = 0; i < openFileLists.count(); i++) {
      if (openFileLists.at(i) == PlistFileName) {
        newfile = false;

#if QT_VERSION_MAJOR < 6
        setActiveWindow(wdlist.at(i));
#else
        wdlist.at(i)->activateWindow();
#endif

        wdlist.at(i)->raise();

        break;
      }
    }

    if (newfile) {
      MainWindow* mw = new MainWindow();
      mw_one = mw;
      mw->openFile(PlistFileName);
      mw->show();
      openFileLists.push_back(PlistFileName);

      wdlist.push_back(mw);
    }
  }
}
