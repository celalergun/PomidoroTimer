#include "mainwindow.h"
#include "ui_mainwindow.h"

void CenterWindow(QMainWindow *w)
{
    QScreen* s= QGuiApplication::screens()[0];
    const QRect sr = s->availableGeometry();
    const QRect wr({}, w->frameSize().boundedTo(sr.size()));

    w->resize(wr.size());
    w->move(sr.center() - wr.center());
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // read icon from resource
    auto icon = QIcon(":/clock.ico");
    // remove window title and make it stay-on-top
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    // create a menu item for system tray icon
    auto exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, &QAction::triggered, [this]()
    {
        closing = true;
        close();
    });

    dndAction = new QAction(tr("&DND"), this);
    dndAction->setCheckable(true);
    connect(dndAction, &QAction::triggered, this, QOverload<>::of(&MainWindow::DndClicked));

    // create system tray menu
    auto trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(dndAction);
    trayIconMenu->addAction(exitAction);

    // create system tray icon and add menu to it
    auto sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setContextMenu(trayIconMenu);
    sysTrayIcon->setIcon(icon);
    sysTrayIcon->setToolTip("Pomidoro timer");
    sysTrayIcon->show();

    // system tray icon click responder
    connect(sysTrayIcon, &QSystemTrayIcon::activated, [this](auto reason)
    {
       if (reason == QSystemTrayIcon::Trigger)
       {
           if (!isVisible())
           {
               show();
               CenterWindow(this);
               activateWindow();
           }
           if (isMinimized())
           {
               showNormal();
           }
       }
    });

    // center window and update clock
    CenterWindow(this);
    UpdateClock();

    // create a timer to check current time
    timer = new QTimer(this);
    closing = false;
    dnd = false;
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::UpdateClock));
    timer->start(1000);
}

void MainWindow::DndClicked()
{
    dnd = !dnd;
    dndAction->setChecked(dnd);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // if we clicked on the Exit menu
    if(closing)
    {
        event->accept();
    }
    // otherwise just hide the window
    else
    {
        this->hide();
        event->ignore();
    }
}

void MainWindow::UpdateClock()
{
    // update time information on screen
    QDateTime current = QDateTime::currentDateTime();
    QString s = current.toString("HH:mm:ss");
    ui->lblClock->setText(s);

    // check if it is the beginning of the new hour and the window is not visible
    // also check to see if we are in DND mode
    if (!dnd && s.endsWith("00:00") && !isVisible())
    {
        CenterWindow(this);
        show();
        // load sound from resource
        QSoundEffect player(this);
        player.setSource(QUrl("qrc:/shwup.wav"));
        player.play();
        // do not leave this method until the sound finishes
        // if we leave the method before it finishes QSoundEffect will be destroyed and the sound will not be played
        while (player.isPlaying())
            QApplication::processEvents();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_closeButton_clicked()
{
    hide();
}

