#include "addcustomevent.h"
#include "ui_addcustomevent.h"

AddCustomEvent::AddCustomEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCustomEvent)
{
    ui->setupUi(this);
}

AddCustomEvent::~AddCustomEvent()
{
    delete ui;
}

QString AddCustomEvent::GetTime()
{
    return ui->timeEdit->text();
}

QString AddCustomEvent::GetMessage()
{
    return ui->lineEdit->text();
}
