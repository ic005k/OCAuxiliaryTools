#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QBitmap>
#include <QDialog>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTextBrowser>
#include <QTextEdit>

class QLabel;

class Tooltip : public QDialog {
 public:
  Tooltip(QWidget* parent = nullptr);
  ~Tooltip();

  void setMyText(QString strHead, const QString& text);
  void popup(QPoint pos, QString strHead, const QString& text);

 protected:
  bool eventFilter(QObject* obj, QEvent* e) override;
  void closeEvent(QCloseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  QTextEdit* edit;
  int thisWidth = 0;
  int thisHeight = 0;
  int currentHeight = 0;
  QLabel* lblTitle;
  QPropertyAnimation* animation;
  int delay = 650;

 private slots:
};

#endif  // TOOLTIP_H
