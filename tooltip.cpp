#include "tooltip.h"
#include "mainwindow.h"
#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>

extern MainWindow* mw_one;

Tooltip::Tooltip(QWidget* parent)
    : QDialog(parent)
{

    this->setAutoFillBackground(true); // 属性控制小部件背景是否自动填充
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, QColor(255, 255, 205, 160));
    palette.setColor(QPalette::Window, QColor(255, 255, 205, 160));
    palette.setColor(QPalette::Text, QColor(0, 0, 0, 255));
    this->setPalette(palette);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_DeleteOnClose);

    edit = new QTextEdit(this);
    edit->setWordWrapMode(QTextOption::WordWrap);
    edit->setReadOnly(true);

    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setMargin(0);
    mLayout->addWidget(edit);

    this->installEventFilter(this);
    //this->setStyleSheet("QWidget { background: rgba(255 ,248 ,220 ,160); color: black}");

    thisWidth = 500;
    thisHeight = 250;

#ifdef Q_OS_WIN32
    thisWidth = 600;
    thisHeight = 300;
#endif

    this->resize(thisWidth, thisHeight);
}

Tooltip::~Tooltip()
{
}

void Tooltip::setMyText(QString strHead, const QString& text)
{
    QString str;
    QStringList strList;

    if (text.contains("----")) {
        strList = text.split("----");
        if (mw_one->zh_cn) {
            str = strList.at(1);
        } else
            str = strList.at(0);
    } else
        str = text;

    edit->setText(strHead + str.trimmed());

    // 文本高度
    QTextDocument* document = edit->document(); //new QTextDocument(edit);
    document->setTextWidth(thisWidth);
    QTextOption op;
    op.setWrapMode(QTextOption::WordWrap);
    document->setDefaultTextOption(op);
    document->adjustSize();
    QAbstractTextDocumentLayout* layout = document->documentLayout();

    int newHeight = layout->documentSize().height();

    if (newHeight < thisHeight)
        currentHeight = newHeight;
    else
        currentHeight = thisHeight;

    edit->setFixedHeight(currentHeight * 1.05);
    this->setFixedHeight(currentHeight * 1.05);
}

bool Tooltip::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == this) {
        if (QEvent::WindowDeactivate == e->type()) {
            this->close();
            e->accept();
            return true;
        }
    }
    return QWidget::eventFilter(obj, e);
}

void Tooltip::popup(QPoint pos, QString strHead, const QString& text)
{
    Tooltip* t = new Tooltip();
    t->setMyText(strHead, text);

    int newX;
    if (pos.x() + thisWidth > mw_one->getMainWidth())
        newX = pos.x() - thisWidth;
    else
        newX = pos.x();

    pos.setY(pos.y() - t->height());
    pos.setX(newX);

    t->move(pos);
    t->show();
}
