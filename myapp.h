#ifndef MYAPP_H
#define MYAPP_H
#include <QApplication>
#include <QEvent>
#include <QFileOpenEvent>

class MyApplication : public QApplication {
public:
    MyApplication(int& argc, char** argv)
        : QApplication(argc, argv)
    {
    }

    bool event(QEvent* event);

    void new_win();
};

#endif // MYAPP_H
