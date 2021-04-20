#ifndef METHOD_H
#define METHOD_H

#include <QMainWindow>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

class Method : public QMainWindow {
    Q_OBJECT
public:
    explicit Method(QWidget* parent = nullptr);

    void goTable(QTableWidget* table);
    QWidget* getSubTabWidget(int m, int s);

signals:
};

#endif // METHOD_H
