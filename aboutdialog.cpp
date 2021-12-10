#include "aboutdialog.h"

#include "mainwindow.h"
#include "ui_aboutdialog.h"

extern MainWindow* mw_one;
QString CurVerison = "20220023";
QString ocVer = "0.7.6";
QString ocVerDev = "0.7.7";
QString ocFrom, ocFromDev;
bool blDEV = false;

aboutDialog::aboutDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::aboutDialog) {
  ui->setupUi(this);
  ocVerDev = ocVerDev + " [" + tr("DEV") + "]";

  ocFrom =
      "<a "
      "href=\"https://github.com/acidanthera/OpenCorePkg/releases/latest/\">" +
      tr("Source");

  ocFromDev =
      "<a "
      "href=\"https://github.com/acidanthera/OpenCorePkg/actions/runs/"
      "1560483990/\">" +
      tr("Source");

  setMaximumWidth(this->width());
  setMaximumHeight(this->height());
  setMinimumWidth(this->width());
  setMinimumHeight(this->height());

  initInfoStr();
}

aboutDialog::~aboutDialog() { delete ui; }

void aboutDialog::initInfoStr() {
  ui->textBrowser->clear();
  ui->textBrowser->setOpenExternalLinks(true);

  ui->textBrowser->append("OC Auxiliary Tools   V" + CurVerison +
                          " for OpenCore " + ocVer);
  ui->textBrowser->append("");

  QString strUrl1 =
      "<a style='color: blue;' href = "
      "https://github.com/ic005k/QtOpenCoreConfig>OC Auxiliary Tools</"
      "a><br><a style='color: blue;'<br>";

  ui->textBrowser->append(strUrl1);

  QFileInfo appInfo(qApp->applicationFilePath());
  QString strLastModified =
      tr("Last modified: ") +
      appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

  QString strC1 = tr("Contributor: eSaF (England, UK)    [file structure]");
  QString strC2 =
      tr("Contributor: Sherlocks (Seoul, Korea)    [file structure]");
  QString strC3 =
      tr("Contributor: 5T33Z0 (Germany)    [Documentation, user experience and "
         "UI interaction design]");
  QString strC4 = tr("Icon Designer: Mirone (Brazil) ");
  ui->textBrowser->append(strC1);
  ui->textBrowser->append(strC2);
  ui->textBrowser->append(strC3);
  ui->textBrowser->append(strC4);

  QString strTr2 = tr("Details");
  QString strDetails =
      "<a style='color: blue;' href = "
      "https://api.github.com/repos/ic005k/QtOpenCoreConfig/releases/"
      "latest>" +
      strTr2 +
      "</"
      "a><br><a style='color: blue;'<br>";
  ui->textBrowser->append("");
  ui->textBrowser->append(strLastModified + "    " + strDetails);
  if (listDownCount.count() == 4)
    ui->textBrowser->append("Linux: " + listDownCount.at(0) +
                            "  Win: " + listDownCount.at(1) +
                            "  Mac: " + listDownCount.at(2) +
                            "  ClassicalMac: " + listDownCount.at(3));

  ui->textBrowser->append("");
  ui->textBrowser->append(
      tr("(This App is built automatically by Github Actions.)"));
}

void aboutDialog::showDownCount() {
  if (listDownCount.count() == 4)
    ui->textBrowser->append("Linux: " + listDownCount.at(0) +
                            "  Win: " + listDownCount.at(1) +
                            "  Mac: " + listDownCount.at(2) +
                            "  ClassicalMac: " + listDownCount.at(3));
}
