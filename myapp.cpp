#include "myapp.h"
#include "mainwindow.h"

extern QString PlistFileName;
extern QVector<QString> filelist;
extern QWidgetList wdlist;
extern MainWindow* mw_one;

bool MyApplication::event(QEvent* event)
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent* openEvent = static_cast<QFileOpenEvent*>(event);
        PlistFileName = openEvent->file();

        if (mw_one->isActiveWindow())
            mw_one->close();

        new_win();
    }

    return QApplication::event(event);
}

void MyApplication::new_win()
{
    if (!PlistFileName.isEmpty()) {
        bool newfile = true;
        for (int i = 0; i < filelist.count(); i++) {
            if (filelist.at(i) == PlistFileName) {
                newfile = false;
                setActiveWindow(wdlist.at(i));
                wdlist.at(i)->raise();

                break;
            }
        }

        if (newfile) {
            MainWindow* mw = new MainWindow();
            mw_one = mw;
            mw->openFile(PlistFileName);
            mw->show();
            filelist.push_back(PlistFileName);

            wdlist.push_back(mw);
        }
    }
}
