#ifndef DLGMOUNTESP_H
#define DLGMOUNTESP_H

#include <QDialog>

namespace Ui {
class dlgMountESP;
}

class dlgMountESP : public QDialog {
  Q_OBJECT

 public:
  explicit dlgMountESP(QWidget* parent = nullptr);
  ~dlgMountESP();
  Ui::dlgMountESP* ui;

  void mountESP(bool openConfig);
 private slots:
  void on_btnMount_clicked();

  void on_btnMountOpenConfig_clicked();

 private:
};

#endif  // DLGMOUNTESP_H
