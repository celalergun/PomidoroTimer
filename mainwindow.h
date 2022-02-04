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
    QAction *dndAction;
    void UpdateClock();
    void DndClicked();
    bool closing;
    bool dnd;
protected:
    void closeEvent(QCloseEvent*) override;
private slots:
    void on_closeButton_clicked();
};
#endif // MAINWINDOW_H
