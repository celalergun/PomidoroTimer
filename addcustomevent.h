#ifndef ADDCUSTOMEVENT_H
#define ADDCUSTOMEVENT_H

#include <QDialog>
#include <QString>

namespace Ui {
class AddCustomEvent;
}

class AddCustomEvent : public QDialog
{
    Q_OBJECT

public:
    explicit AddCustomEvent(QWidget *parent = nullptr);
    ~AddCustomEvent();
    QString GetTime();
    QString GetMessage();
private:
    Ui::AddCustomEvent *ui;
};

#endif // ADDCUSTOMEVENT_H
