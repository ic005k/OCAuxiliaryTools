#include "aboutdialog.h"

#include "Method.h"
#include "mainwindow.h"
#include "ui_aboutdialog.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
extern QString strAppName, strIniFile;
QString CurVersion = "20240004";
QString ocVer = "0.8.8";
QString ocVerDev = "0.8.9";
QString ocFrom, ocFromDev, strOCFrom, strOCFromDev;
bool blDEV = false;

aboutDialog::aboutDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::aboutDialog) {
  ui->setupUi(this);

  strOCFromDev =
      "https://github.com/acidanthera/OpenCorePkg/actions/runs/1808597752/";

  strOCFrom = "https://github.com/acidanthera/OpenCorePkg/releases/latest";
  ocFrom = "<a href=\"" + strOCFrom + "\"" + "> " + tr(" Source ");

  strIniFile =
      QDir::homePath() + "/.config/" + strAppName + "/" + strAppName + ".ini";
  QSettings Reg(strIniFile, QSettings::IniFormat);
  QString ver = Reg.value("ocVer").toString();
  if (ver != "") {
    ocVer = ver;
  }
  ver = Reg.value("ocVerDev").toString();
  if (ver > ocVerDev) {
    ocVerDev = ver;
  }

  QString DevSource = Reg.value("DevSource").toString();
  if (DevSource.trimmed() == "") {
    ocFromDev =
        "";  // "<a href=\"" + strOCFromDev + "\"" + "> " + tr(" Source ");
  } else {
    ocFromDev =
        "<a href=\"" + DevSource.trimmed() + "\"" + "> " + tr(" Source ");
  }

  setMaximumWidth(this->width());
  setMaximumHeight(this->height());
  setMinimumWidth(this->width());
  setMinimumHeight(this->height());

  ui->label->setFixedHeight(85);
  ui->label->setFixedWidth(85);
  ui->label->setText("");

  ui->label->setStyleSheet(
      "QLabel{"
      "border-image:url(:/icon.png) 4 4 4 4 stretch stretch;"
      "}");

  if (!blDEV)
    ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                            " for OpenCore " + ocVer);
  else
    ui->lblVersion->setText(tr("Version") + "  " + CurVersion +
                            " for OpenCore " + ocVerDev);
  QFont font;
  font.setBold(true);
  font.setPixelSize(25);
  ui->lblOCAT->setFont(font);
}

aboutDialog::~aboutDialog() { delete ui; }

void aboutDialog::initInfoStr() {}
