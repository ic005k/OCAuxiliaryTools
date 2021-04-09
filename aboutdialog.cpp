#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    setMaximumWidth(this->width());
    setMaximumHeight(this->height());
    setMinimumWidth(this->width());
    setMinimumHeight(this->height());

    ui->textBrowser->setOpenExternalLinks(true);

    QString strUrl1 = "<a style='color: blue;' href = "
                      "https://github.com/ic005k/QtOpenCoreConfig>OC Auxiliary Tools</"
                      "a><br><a style='color: blue;'<\n>";

    ui->textBrowser->append(strUrl1);

    QString str0 = tr("Discussion area");
    QString strUrl2 = "<a style='color: blue;' href = "
                      "https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-configuration-file-editor>"
        + str0 + "</"
                 "a><br><a style='color: blue;'<\n>";

    ui->textBrowser->append(strUrl2);

    QFileInfo appInfo(qApp->applicationFilePath());
    QString strLastModified = tr("Last modified: ") + appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

    QString strC1 = tr("Contributor: eSaF (England, UK)    [file structure]");
    QString strC2 = tr("Contributor: Sherlocks (Seoul, Korea)    [file structure]");
    QString strC3 = tr("Icon Designer: Mirone (Brazil) ");
    ui->textBrowser->append(strC1);
    ui->textBrowser->append(strC2);
    ui->textBrowser->append(strC3);

    ui->textBrowser->append("");
    ui->textBrowser->append(strLastModified);

    ui->textBrowser->append("");
    ui->textBrowser->append(tr("(This App is built automatically by Github Actions.)"));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
