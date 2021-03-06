#include "mainwindow.h"
#include "ui_mainwindow.h"

void CenterWindow(QMainWindow *w)
{
    QScreen* s= QGuiApplication::screens()[0];
    const QRect sr = s->availableGeometry();
    const QRect wr({}, w->frameSize().boundedTo(sr.size()));
    w->move(sr.center() - wr.center());
}

void MainWindow::CreateMenus()
{
    // create a menu item for system tray icon
    auto exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, &QAction::triggered, [this]()
    {
        closing = true;
        close();
    });

    DndAction = new QAction(tr("&DND"), this);
    DndAction->setCheckable(true);
    connect(DndAction, &QAction::triggered, [this]()
    {
        dnd = !dnd;
        DndAction->setChecked(dnd);
    });

    MuteAction = new QAction(tr("&No sound"), this);
    MuteAction->setCheckable(true);
    connect(MuteAction, &QAction::triggered, [this]()
    {
        mute = !mute;
        MuteAction->setChecked(mute);
    });

    CustomEventAction = new QAction(tr("&Add custom event"), this);
    connect(CustomEventAction, &QAction::triggered, [this]()
    {
        DisplayCustomEventWindow();
    });

    // create a system tray menu and add the items to the menu
    QMenu *trayIconMenu = CreateSystemTrayMenu();
    trayIconMenu->addAction(DndAction);
    trayIconMenu->addAction(MuteAction);
    trayIconMenu->addSeparator();

    // custom events
    trayIconMenu->addAction(CustomEventAction);
    customEventsMenu = trayIconMenu->addMenu("Custom events");
    customEventsMenu->setEnabled(false);

    // now we can add exit menu as the last item
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);
}

void MainWindow::DisplayCustomEventWindow()
{
    bool wasHidden = false;
    if (!this->isVisible())
    {
        wasHidden = true;
        this->show();
    }

    AddCustomEvent *addCustomEventWindow = new AddCustomEvent(this) ;
    auto result = addCustomEventWindow->exec();
    if (result == QDialog::Accepted)
    {
        auto time = addCustomEventWindow->GetTime();
        auto message = addCustomEventWindow->GetMessage();
        auto caption = QString("%1->%2").arg(time, message);
        QAction *addThis = new QAction(caption, this);
        addThis->setStatusTip(message);
        customEventsMenu->addAction(addThis);
        customEventsMenu->setEnabled(true);
        customEventsList.append(QPair<QString, QString>(time, message));
    }

    if (wasHidden)
        this->hide();
}

QMenu *MainWindow::CreateSystemTrayMenu()
{
    // read icon from resource
    auto icon = QIcon(":/clock.ico");

    // create system tray main menu
    auto trayIconMenu = new QMenu(this);

    // create system tray icon and add menu to it
    sysTrayIcon = new QSystemTrayIcon(this);
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
                sysTrayIcon->hide();
                CenterWindow(this);
                activateWindow();
            }
            if (isMinimized())
            {
                showNormal();
            }
        }
    });
    return trayIconMenu;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // remove window title and make it stay-on-top
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    CreateMenus();
    // center window and update clock
    CenterWindow(this);
    UpdateClock();

    // create a timer to check current time
    timer = new QTimer(this);
    closing = false;
    dnd = false;
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::UpdateClock));
    timer->start(1000);
    firstWidth = this->size().width();
    firstHeight = this->size().height();
    int currentHour = QDateTime::currentDateTime().time().hour();
    if (currentHour < 12)
        ui->lblMessage->setText("Good Morning!");
    else if (currentHour < 17)
        ui->lblMessage->setText("Good Afternoon!");
    else
        ui->lblMessage->setText("Good Evening!");

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(closing) // check to see if we clicked on the Exit menu
    {
        event->accept();
    }
    else // otherwise just hide the window
    {
        sysTrayIcon->show();
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

    bool newHour = s.endsWith("00:00");
    bool isInCustomEvents = false;
    QString message = "";
    foreach(auto item, customEventsList)
    {
        if (item.first == s)
        {
            isInCustomEvents = true;
            message = item.second;
            break;
        }
    }
    // check if it is the beginning of the new hour and the window is not visible
    // also check to see if we are in DND mode
    if (!dnd && (newHour || isInCustomEvents) && !isVisible())
    {
        ui->lblMessage->setText(message);
        CenterWindow(this);
        show();
        sysTrayIcon->hide();

        // load sound from resource
        if (!mute)
        {
            QSoundEffect player(this);
            player.setSource(QUrl("qrc:/shwup.wav"));
            player.play();
            // do not leave this method until the sound finishes
            // if we leave the method before it finishes QSoundEffect will be destroyed and the sound will not be played
            while (player.isPlaying())
                QApplication::processEvents();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_closeButton_clicked()
{
    sysTrayIcon->show();
    hide();
}

