#ifndef BALLOONTIP_H
#define BALLOONTIP_H

#include <QMessageBox>
#include <QPixmap>
#include <QTimer>
#include <QTimerEvent>
#include <QWidget>

class BalloonTip : public QWidget {
  Q_OBJECT
 public:
  QTimer* timer;
  static void showBalloon(QMessageBox::Icon icon, const QString& title,
                          const QString& msg, const QPoint& pos, int timeout,
                          bool showArrow = true, int arrowDir = 12);
  static void hideBalloon();
  static bool isBalloonVisible();
  static void updateBalloonPosition(const QPoint& pos);

 private:
  BalloonTip(QMessageBox::Icon icon, const QString& title, const QString& msg);
  ~BalloonTip();
  void balloon(const QPoint&, int, bool, int arrowDir);

 protected:
  void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
  void timerEvent(QTimerEvent* e) Q_DECL_OVERRIDE;

 private slots:
  void closeTip();

 private:
  QPixmap pixmap;
  int timerId;
  int eventTimerId;
  bool showArrow;
  bool enablePressEvent;
  int arrowDir;
};

#endif  // BALLOONTIP_H
