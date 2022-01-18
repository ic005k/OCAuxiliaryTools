#include "aboutdialog.h"

#include "mainwindow.h"
#include "ui_aboutdialog.h"
#include "ui_mainwindow.h"

extern MainWindow* mw_one;
QString CurVerison = "20220074";
QString ocVer = "0.7.7";
QString ocVerDev = "0.7.8";
QString ocFrom, ocFromDev, strOCFrom, strOCFromDev;
bool blDEV = false;

aboutDialog::aboutDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::aboutDialog) {
  ui->setupUi(this);

  strOCFrom = "https://github.com/acidanthera/OpenCorePkg/releases/tag/0.7.7";
  strOCFromDev =
      "https://github.com/acidanthera/OpenCorePkg/actions/runs/1703949023/";

  setMaximumWidth(this->width());
  setMaximumHeight(this->height());
  setMinimumWidth(this->width());
  setMinimumHeight(this->height());

  ocVerDev = ocVerDev + " [" + tr("DEV") + "]";
  ocFromDev = "<a href=\"" + strOCFromDev + "\"" + "> " + tr(" Source ");
  ocFrom = "<a href=\"" + strOCFrom + "\"" + "> " + tr(" Source ");

  ui->label->setFixedHeight(85);
  ui->label->setFixedWidth(85);
  ui->label->setText("");

  ui->label->setStyleSheet(
      "QLabel{"
      "border-image:url(:/icon.png) 4 4 4 4 stretch stretch;"
      "}");

  if (!blDEV)
    ui->lblVersion->setText(tr("Version") + "  " + CurVerison +
                            " for OpenCore " + ocVer);
  else
    ui->lblVersion->setText(tr("Version") + "  " + CurVerison +
                            " for OpenCore " + ocVerDev);
  QFont font;
  font.setBold(true);
  font.setPixelSize(25);
  ui->lblOCAT->setFont(font);
}

aboutDialog::~aboutDialog() { delete ui; }

void aboutDialog::initInfoStr() {}
