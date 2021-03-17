#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QDialog>
#include <QTextEdit>

class QLabel;

class Tooltip : public QDialog {
public:
    Tooltip(QWidget* parent = nullptr);
    ~Tooltip();

    void setText(const QString& text);
    void popup(QPoint pos, const QString& text);

protected:
    bool eventFilter(QObject* obj, QEvent* e);

private:
    QTextEdit* edit;
    int thisWidth = 0;
    int thisHeight = 0;
};

#endif // TOOLTIP_H
