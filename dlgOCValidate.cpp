#include "dlgOCValidate.h"
#include "mainwindow.h"
#include "ui_dlgOCValidate.h"

extern MainWindow* mw_one;

dlgOCValidate::dlgOCValidate(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgOCValidate)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);

    ui->btnGo->setVisible(false);
    ui->btnClose->setVisible(false);

    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction* copyAction = new QAction(tr("Copy"));
    QAction* searchAction = new QAction(tr("Search..."));

    QMenu* copyMenu = new QMenu(this);
    //copyMenu->addAction(copyAction);
    copyMenu->addAction(searchAction);

    connect(copyAction, &QAction::triggered, [=]() {
        QString str = ui->textEdit->textCursor().selectedText();

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(str.trimmed());
    });

    connect(searchAction, &QAction::triggered, [=]() {
        QString str = ui->textEdit->textCursor().selectedText().trimmed();
        mw_one->startSearch(str);
    });

    connect(ui->textEdit, &QTextEdit::customContextMenuRequested, [=](const QPoint& pos) {
        Q_UNUSED(pos);

        QString str = ui->textEdit->textCursor().selectedText().trimmed();
        if (str.count() > 0) {
            searchAction->setEnabled(true);
            copyAction->setEnabled(true);
        } else {
            searchAction->setEnabled(false);
            copyAction->setEnabled(false);
        }

        copyMenu->exec(QCursor::pos());
    });
}

dlgOCValidate::~dlgOCValidate()
{
    delete ui;
}

void dlgOCValidate::setTextOCV(QString str)
{
    ui->textEdit->clear();
    ui->textEdit->append(str);
}

void dlgOCValidate::on_btnClose_clicked()
{
    this->close();
}

void dlgOCValidate::setGoEnabled(bool enabled)
{
    if (enabled)
        ui->btnGo->setEnabled(true);
    else
        ui->btnGo->setEnabled(false);
}
