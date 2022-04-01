#ifndef DLGNEWKEYFIELD_H
#define DLGNEWKEYFIELD_H

#include <QDialog>
#include <QFrame>
#include <QInputDialog>
#include <QRegularExpression>
#include <QWidget>

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
  static QLineEdit* add_LineEdit(QWidget* tab, QString ObjectName,
                                 QString text);
  static void add_CheckBox(QWidget* tab, QString ObjectName, QString text);
  static QStringList getAllNewKey();
  static QList<int> getKeyMainSub(QString Key);
  static QStringList check_SampleFile(QVariantMap mapTatol, QWidget* tab,
                                      QString MainName, QString SubName);
  static QStringList get_KeyTypeValue(QVariantMap mapTatol, QString MainName,
                                      QString SubName);
  static QWidgetList get_AllLabelList(QWidget* tab);
  static QString get_WidgetText(QWidget* w);
  static void set_BtnHide(QWidget* tab, QString Key);
 private slots:
  void on_btnAdd_clicked();

 private:
  static void removeKey(QString ObjectName);
  static void set_WidgetHide(QWidgetList listOCATWidget, QString Key);
  static void set_LblHide(QWidget* tab, QString Key);
};

#endif  // DLGNEWKEYFIELD_H
