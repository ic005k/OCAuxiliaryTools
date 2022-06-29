#ifndef DLGOCVALIDATE_H
#define DLGOCVALIDATE_H

#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>

namespace Ui {
class dlgOCValidate;
}

class dlgOCValidate : public QDialog {
  Q_OBJECT

 public:
  explicit dlgOCValidate(QWidget* parent = nullptr);
  ~dlgOCValidate();
  void setTextOCV(QString str);
  void setGoEnabled(bool enabled);
  Ui::dlgOCValidate* ui;

  void goMainList(QString value, QString subValue);
 private slots:
  void on_btnClose_clicked();

  void on_btnGo_clicked();

  void on_btnCreateVault_clicked();

  void readData();

  void readResult(int exitCode);

 private:
  QProcess* process;
};

#endif  // DLGOCVALIDATE_H
