#ifndef DLGNEWKEYFIELD_H
#define DLGNEWKEYFIELD_H

#include <QDialog>

namespace Ui {
class dlgNewKeyField;
}

class dlgNewKeyField : public QDialog {
  Q_OBJECT

 public:
  explicit dlgNewKeyField(QWidget* parent = nullptr);
  ~dlgNewKeyField();
  Ui::dlgNewKeyField* ui;

  static void saveNewKey(QString ObjectName, int main, int sub);
  static void readNewKey(QWidget* tab, QString Key);
  static void add_LineEdit(QWidget* tab, QString ObjectName, QString text);
  static void add_CheckBox(QWidget* tab, QString ObjectName, QString text);
  static QStringList getAllNewKey();
  static QList<int> getKeyMainSub(QString Key);
 private slots:
  void on_btnAdd_clicked();

 private:
};

#endif  // DLGNEWKEYFIELD_H
