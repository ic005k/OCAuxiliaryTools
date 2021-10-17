#include "aboutdialog.h"

#include "mainwindow.h"
#include "ui_aboutdialog.h"

extern MainWindow* mw_one;
QString CurVerison = "20211021";
QString ocVer = "0.7.5";

aboutDialog::aboutDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::aboutDialog) {
  ui->setupUi(this);

  setMaximumWidth(this->width());
  setMaximumHeight(this->height());
  setMinimumWidth(this->width());
  setMinimumHeight(this->height());

  initInfoStr();
}

aboutDialog::~aboutDialog() { delete ui; }

void aboutDialog::initInfoStr() {
  ui->textBrowser->setOpenExternalLinks(true);

  ui->textBrowser->append("OC Auxiliary Tools   V" + CurVerison +
                          " for OpenCore " + ocVer);
  ui->textBrowser->append("");

  QString strUrl1 =
      "<a style='color: blue;' href = "
      "https://github.com/ic005k/QtOpenCoreConfig>OC Auxiliary Tools</"
      "a><br><a style='color: blue;'<\n>";

  ui->textBrowser->append(strUrl1);

  QString str0 = tr("Discussion area");
  QString strUrl2 =
      "<a style='color: blue;' href = "
      "https://www.insanelymac.com/forum/topic/"
      "344752-open-source-cross-platform-opencore-configuration-file-editor>" +
      str0 +
      "</"
      "a><br><a style='color: blue;'<\n>";

  ui->textBrowser->append(strUrl2);

  QFileInfo appInfo(qApp->applicationFilePath());
  QString strLastModified =
      tr("Last modified: ") +
      appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

  QString strC1 = tr("Contributor: eSaF (England, UK)    [file structure]");
  QString strC2 =
      tr("Contributor: Sherlocks (Seoul, Korea)    [file structure]");
  QString strC3 = tr("Icon Designer: Mirone (Brazil) ");
  ui->textBrowser->append(strC1);
  ui->textBrowser->append(strC2);
  ui->textBrowser->append(strC3);

  QString strDetails =
      "<a style='color: blue;' href = "
      "https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/"
      "latest>Details</"
      "a><br><a style='color: blue;'<\n>";

  ui->textBrowser->append("");
  ui->textBrowser->append(strLastModified + "    " + strDetails);

  ui->textBrowser->append("");
  ui->textBrowser->append(
      tr("(This App is built automatically by Github Actions.)"));
}
