#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QKeySequence>
#include <QSoundEffect>
#include <QStyle>
#include <QWidget>
#include <QList>
#include <QPair>
#include "addcustomevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer = nullptr;
    QAction *DndAction;
    QAction *MuteAction;
    QAction *CustomEventAction;
    QMenu *customEventsMenu;
    QSystemTrayIcon *sysTrayIcon;
    void UpdateClock();
    void CreateMenus();
    QMenu *CreateSystemTrayMenu();
    void DisplayCustomEventWindow();
    bool closing;
    bool dnd;
    bool mute;
    QList<QPair<QString, QString>> customEventsList;
    int firstWidth, firstHeight;
protected:
    void closeEvent(QCloseEvent*) override;
private slots:
    void on_closeButton_clicked();
};
#endif // MAINWINDOW_H
