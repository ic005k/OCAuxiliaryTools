#include "tooltip.h"
#include "mainwindow.h"
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
    edit->setReadOnly(true);

    QHBoxLayout* mLayout = new QHBoxLayout(this);
    mLayout->setMargin(0);
    mLayout->addWidget(edit);

    this->installEventFilter(this);
    //this->setStyleSheet("QWidget { background: rgba(255 ,248 ,220 ,160); color: black}");

    thisWidth = 450;
    thisHeight = 260;
    this->resize(thisWidth, thisHeight);
}

Tooltip::~Tooltip()
{
}

void Tooltip::setText(const QString& text)
{
    edit->setText(text);
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

void Tooltip::popup(QPoint pos, const QString& text)
{
    Tooltip* t = new Tooltip();
    t->setText(text);

    int newX;
    if (pos.x() + thisWidth > mw_one->getMainWidth())
        newX = pos.x() - thisWidth;
    else
        newX = pos.x();

    pos.setY(pos.y() - thisHeight);
    pos.setX(newX);

    t->resize(thisWidth, thisHeight);
    t->move(pos);
    t->show();
}
