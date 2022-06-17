#include "dlginfo.h"

#include "mainwindow.h"
#include "ui_dlginfo.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;

dlgInfo::dlgInfo(QWidget* parent) : QDialog(parent), ui(new Ui::dlgInfo) {
  ui->setupUi(this);
}

dlgInfo::~dlgInfo() { delete ui; }

void dlgInfo::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  // if (!mw_one->dlgSyncOC->dlEnd) {
  //   mw_one->dlgSyncOC->dlEnd = true;
  //   mw_one->dlgSyncOC->on_btnStop_clicked();
  // }
}
