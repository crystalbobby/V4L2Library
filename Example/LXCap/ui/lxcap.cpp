#include "lxcap.h"
#include "ui_lxcap.h"
#include "filterwindow.h"
#include "pinwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imgshow->setScaledContents(true);
    connect(ui->menuBar,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*)));

    timerShowImg = new QTimer(this);
    connect(timerShowImg,SIGNAL(timeout()),this,SLOT(refreshImg()));

    actRefresh = new QAction(this);
    actRefresh->setText("Refresh");
    connect(actRefresh,SIGNAL(triggered(bool)),this,SLOT(refreshDev()));
    if(getDevs() == 0){
        changePin(640,480,60);
    }

    timerShootImg = new QTimer(this);
    connect(timerShootImg,SIGNAL(timeout()),this,SLOT(on_actionSnap_triggered()));
    connect(ui->statusBar,SIGNAL(messageChanged(QString)),this,SLOT(showStatus(QString)));
}

void MainWindow::showStatus(QString txt){
    if(txt.toStdString() == "" && timerShowImg->isActive())
        this->statusBar()->showMessage(QString::fromStdString(cam->devName)+" (video"+QString::number(CamNum)+")    |   "+\
                                       QString::number(cam->Width)+" x "+QString::number(cam->Height)+\
                                       " "+QString::number(cam->Fps)+"Hz",0);
}

void MainWindow::trigerMenu(QAction *act){
    int indexVideo = act->text().indexOf("video");
    if(indexVideo != -1){
        cam->releaseMen();
        changeDev(act->text().mid(indexVideo+5).toInt());
    }
}

void MainWindow::on_actionVideo_Capture_Filter_triggered()
{
    FilterWindow *setw = new FilterWindow(this);
    setw->setWindowFlags(Qt::Window);
    setw->cam = cam;
    setw->readOptions();
    setw->setWindowModality(Qt::ApplicationModal);
    setw->show();
}

void MainWindow::on_actionVideo_Capture_Pin_triggered()
{
    PinWindow *pinw = new PinWindow(this);
    connect(pinw,SIGNAL(commitSets(uint,uint,uint)),this,SLOT(changePin(uint,uint,uint)));
    pinw->setWindowFlags(Qt::Window);
    pinw->FormatType = cam->FormatType;
    pinw->readOptions();
    pinw->setWindowModality(Qt::ApplicationModal);
    pinw->show();
}

void MainWindow::on_actionFull_Screen_triggered()
{
    ui->menuBar->setVisible(false);
    ui->statusBar->setVisible(false);
    this->setWindowState(Qt::WindowFullScreen);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_F11 || event->key() == Qt::Key_Escape){
        this->setWindowState(Qt::WindowNoState);
        ui->statusBar->setVisible(true);
        ui->menuBar->setVisible(true);
    }
}

void MainWindow::on_actionPause_toggled(bool arg1)
{
    if(arg1){
        this->statusBar()->showMessage("Pause");
        timerShowImg->stop();
    }
    else{
        this->statusBar()->showMessage(QString::fromStdString(cam->devName)+" (video"+QString::number(CamNum)+")    |   "+\
                                            QString::number(cam->Width)+" x "+QString::number(cam->Height)+\
                                            " "+QString::number(cam->Fps)+"Hz",0);
        timerShowImg->start();
    }
}

void MainWindow::setSavePath(){
    imgSaveAddr = QFileDialog::getExistingDirectoryUrl(this,tr("Select a directory to save pictures..."));
}

void MainWindow::on_actionSet_save_path_triggered()
{
    setSavePath();
}

void MainWindow::on_actionSnap_triggered()
{
    if(imgSaveAddr.url()==""){
        setSavePath();
        if(imgSaveAddr.isEmpty()) return;
    }
    QDateTime time = QDateTime::currentDateTime();
    QString imgName = QString(imgSaveAddr.toString().mid(7) + "/" + time.toString("MM-dd-yyyy hh:mm:ss.zzz") + ".jpg");
    if(camimg.save(imgName)){
        this->statusBar()->showMessage("Save "+imgName+" successful.");
        QSound::play(":/new/effect/kada.wav");
    }
}

void MainWindow::on_actionTimed_shoot_triggered(bool checked)
{
    if(checked){
        if(imgSaveAddr.url()==""){
            setSavePath();
            if(imgSaveAddr.isEmpty()){
                ui->actionTimed_shoot->setChecked(false); return;
            }
        }
        QString timeSeted = QInputDialog::getText(this,
                    tr( "Timed shooter" ),
                    tr( "Please set a time (second)" ));
        if(timeSeted == ""){
            ui->actionTimed_shoot->setChecked(false); return;
        }
        timerShootImg->start((int)(timeSeted.toFloat()*1000.0));
    }
    else
        timerShootImg->stop();
}

void MainWindow::on_actionHelp_triggered()
{
    const QUrl url("https://wiki77777.github.io/LXCap");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionGithub_triggered()
{
    const QUrl url("https://github.com/Wiki77777/V4L2Library");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionAuthor_s_Blog_triggered()
{
    const QUrl url("http://blog.csdn.net/wikichan");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About LXCap", "LXCap - V4L2 Video Capture Sample\n"
                                      "Version 0.1\n"
                                      "©2017,Wiki Chan 陈炜圻\n\n"
                                      "Webside:\n"
                                      "https://wiki77777.github.io/LXCap\n");
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
