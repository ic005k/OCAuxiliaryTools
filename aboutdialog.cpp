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

    QString strUrl1 = "<a style='color: blue;' href = "
                      "https://github.com/ic005k/QtOpenCoreConfig>OC Auxiliary Tools</"
                      "a><br><a style='color: blue;'<\n><br>";

    ui->linkLabel1->setOpenExternalLinks(true);
    ui->linkLabel1->setText(strUrl1);

    QString str0 = tr("Discussion area");
    QString strUrl2 = "<a style='color: blue;' href = "
                      "https://www.insanelymac.com/forum/topic/344752-open-source-cross-platform-opencore-configuration-file-editor>"
        + str0 + "</"
                 "a><br><a style='color: blue;'<\n><br>";

    ui->linkLabel2->setOpenExternalLinks(true);
    ui->linkLabel2->setText(strUrl2);

    QImage* img = new QImage();

    img->load(":/icon.png");
    ui->lblIcon->setPixmap(QPixmap::fromImage(*img));

    QFileInfo appInfo(qApp->applicationFilePath());
    ui->lblLastModi->setText(tr("Last modified: ") + appInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
