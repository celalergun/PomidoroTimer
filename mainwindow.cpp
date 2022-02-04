#include "mainwindow.h"
#include "ui_mainwindow.h"

void center(QMainWindow *w)
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
    auto icon = QIcon(":/clock.ico");
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    auto exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, &QAction::triggered, [this]()
    {
        closing = true;
        close();
    });

    auto trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(exitAction);

    auto sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setContextMenu(trayIconMenu);
    sysTrayIcon->setIcon(icon);
    sysTrayIcon->setToolTip("Pomidoro timer");
    sysTrayIcon->show();

    connect(sysTrayIcon, &QSystemTrayIcon::activated, [this](auto reason)
    {
       if (reason == QSystemTrayIcon::Trigger)
       {
           if (!isVisible())
           {
               show();
               center(this);
               activateWindow();

           }
           if (isMinimized())
           {
               showNormal();
           }
       }
    });


    updateClock();
    timer = new QTimer(this);
    closing = false;
    dnd = false;
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updateClock));
    timer->start(1000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(closing)
    {
        event->accept();
    }
    else
    {
        this->hide();
        event->ignore();
    }
}

void MainWindow::updateClock()
{
    center(this);
    QDateTime current = QDateTime::currentDateTime();
    QString s = current.toString("HH:mm:ss");
    ui->label->setText(s);
    if (s.endsWith("00:00") && !isVisible())
    {
        show();
        QSoundEffect player(this);
        player.setSource(QUrl("qrc:/shwup.wav"));
        player.play();
        while (player.isPlaying())
            QApplication::processEvents();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    hide();
}

