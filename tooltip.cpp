#include "tooltip.h"

#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "mainwindow.h"

extern MainWindow* mw_one;

Tooltip::Tooltip(QWidget* parent) : QDialog(parent) {
  setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  animation = new QPropertyAnimation(this, "windowOpacity");
  delay = 600;

  this->setAutoFillBackground(true);
  QPalette palette = this->palette();
  palette.setColor(QPalette::Base, QColor(255, 255, 225, 255));
  palette.setColor(QPalette::Window, QColor(255, 255, 225, 255));
  palette.setColor(QPalette::Text, QColor(0, 0, 0, 255));
  this->setPalette(palette);

  edit = new QTextEdit(this);
  edit->setWordWrapMode(QTextOption::WordWrap);
  edit->setReadOnly(true);
  edit->setFrameStyle(QFrame::NoFrame);

  QHBoxLayout* mLayout = new QHBoxLayout(this);
  mLayout->setMargin(0);
  lblTitle = new QLabel();
  QFont font;
  font.setBold(true);
  lblTitle->setFont(font);
  lblTitle->setFixedHeight(40);
  lblTitle->setFixedWidth(40);
  lblTitle->setText("");
  lblTitle->setStyleSheet(
      "QLabel{"
      "border-image:url(:/icon/tip.png) 4 4 4 4 stretch stretch;"
      "}");

  mLayout->addWidget(lblTitle);

  mLayout->addWidget(edit);

  this->installEventFilter(this);

  thisWidth = 500;
  thisHeight = 250;

#ifdef Q_OS_WIN32
  thisWidth = 600;
  thisHeight = 300;
#endif

  this->resize(thisWidth, thisHeight);
}

Tooltip::~Tooltip() {}

void Tooltip::setMyText(QString strHead, const QString& text) {
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

  this->setWindowTitle(strHead);
  edit->setText(str.trimmed());

  // 文本高度
  QTextDocument* document = edit->document();  // new QTextDocument(edit);
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

  if (currentHeight < 40) currentHeight = 40;
  edit->setFixedHeight(currentHeight * 1.05);
  this->setFixedHeight(currentHeight * 1.05);
}

bool Tooltip::eventFilter(QObject* obj, QEvent* e) {
  if (obj == this) {
    if (QEvent::WindowDeactivate == e->type()) {
      animation->setDuration(delay);
      animation->setStartValue(1);
      animation->setEndValue(0);
      connect(animation, SIGNAL(finished()), this, SLOT(close()));
      animation->start();

      e->accept();
      return true;
    }
  }
  return QWidget::eventFilter(obj, e);
}

void Tooltip::popup(QPoint pos, QString strHead, const QString& text) {
  if (text.trimmed().length() == 0) return;

  this->installEventFilter(this);
  this->setMyText(strHead, text);

  int newX;
  if (pos.x() + thisWidth > mw_one->getMainWidth())
    newX = pos.x() - thisWidth;
  else
    newX = pos.x();

  pos.setY(pos.y() - this->height());
  pos.setX(newX - 10);

  this->move(pos);
  this->show();
  QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
  animation->setDuration(delay);
  animation->setStartValue(0);
  animation->setEndValue(1);
  animation->start();

  int t1 = text.length() * 35;
  qDebug() << text.length();
  if (t1 < 5000) t1 = 5000;
  QElapsedTimer t0;
  t0.start();
  while (t0.elapsed() < t1) {
    QCoreApplication::processEvents();
  }

  animation->setDuration(delay);
  animation->setStartValue(1);
  animation->setEndValue(0);
  animation->start();
  connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

void Tooltip::paintEvent(QPaintEvent* event) {
  return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(QColor(0xff, 0xff, 0xe1)));
  // painter.setBrush(QBrush(Qt::lightGray));
  painter.setPen(Qt::black);
  QRect rect = this->rect();
  rect.setWidth(rect.width() - 1);
  rect.setHeight(rect.height() - 1);
  painter.drawRoundedRect(rect, 15, 15);

  QWidget::paintEvent(event);
}

void Tooltip::closeEvent(QCloseEvent* event) { Q_UNUSED(event); }
